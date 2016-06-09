
#ifndef LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_IMAGEMODEL_HH_
#define LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_IMAGEMODEL_HH_

#include <cmath>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <Python.h>
#include <structmember.h>

#include "../private/buffer_t.h"
#include "../buffer.hpp"
#include "../check.hh"
#include "../gil.hpp"
#include "../gil-io.hpp"
#include "../detail.hpp"
#include "../numpy.hpp"
#include "../options.hpp"
#include "../pybuffer.hpp"
#include "../pycapsule.hpp"
#include "../typecode.hpp"
#include "../hybrid.hh"
#include "base.hh"

#include <libimread/ext/errors/demangle.hh>
#include <libimread/ext/filesystem/path.h>
#include <libimread/ext/filesystem/temporary.h>
#include <libimread/ext/base64.hh>
#include <libimread/errors.hh>
#include <libimread/hashing.hh>
#include <libimread/pixels.hh>

namespace py {
    
    namespace ext {
        
        using im::byte;
        using im::options_map;
        using im::Image;
        using im::ImageFormat;
        using im::HalideNumpyImage;
        using im::ArrayImage;
        using im::HybridFactory;
        using im::ArrayFactory;
        
        using filesystem::path;
        using filesystem::NamedTemporaryFile;
        
        template <typename ImageType,
                  typename BufferType = buffer_t,
                  typename FactoryType = typename ImageType::factory_t>
        struct ImageModelBase : public ModelBase {
            
            using shared_image_t = std::shared_ptr<ImageType>;
            using weak_image_t = std::weak_ptr<ImageType>;
            
            struct BufferModel : public BufferModelBase<BufferType> {
                
                using base_t = BufferModelBase<BufferType>;
                
                void* operator new(std::size_t newsize) {
                    PyTypeObject* type = FactoryType::buffer_type();
                    return reinterpret_cast<void*>(type->tp_alloc(type, 0));
                }
                
                void operator delete(void* voidself) {
                    BufferModel* self = reinterpret_cast<BufferModel*>(voidself);
                    PyObject* pyself = reinterpret_cast<PyObject*>(voidself);
                    if (self->weakrefs != nullptr) { PyObject_ClearWeakRefs(pyself); }
                    self->cleanup();
                    FactoryType::buffer_type()->tp_free(pyself);
                }
                
                weak_image_t image;
                
                BufferModel()
                    :base_t()
                    {}
                
                explicit BufferModel(shared_image_t shared_image)
                    :base_t(shared_image->buffer_ptr())
                    ,image(shared_image)
                    {}
                
                BufferModel(BufferModel const& other)
                    :base_t(other.internal.get())
                    ,image(other.image)
                    {}
                
                /// tag dispatch, reinterpret, depointerize, copy-construct
                explicit BufferModel(PyObject* other)
                    :BufferModel(*reinterpret_cast<BufferModel*>(other))
                    {}
                
                Py_ssize_t  __len__() {
                    Py_ssize_t out;
                    {
                        py::gil::release nogil;
                        shared_image_t strong = image.lock();
                        out = strong->size();
                    }
                    return out;
                }
                
                PyObject*   __index__(Py_ssize_t idx, int tc = NPY_UINT8) {
                    Py_ssize_t siz;
                    std::size_t nidx;
                    shared_image_t strong;
                    {
                        py::gil::release nogil;
                        strong = image.lock();
                        siz = strong->size();
                        nidx = static_cast<std::size_t>(idx);
                    }
                    if (siz <= idx || idx < 0) {
                        PyErr_SetString(PyExc_IndexError,
                            "index out of range");
                        return nullptr;
                    }
                    return py::detail::image_typed_idx(strong, tc, nidx);
                }
                
                template <typename Pointer = PyArrayInterface,
                          typename = std::nullptr_t>
                py::cob::single_destructor_t array_destructor() const {
                    return [](void* voidptr) {
                        Pointer* pointer = (Pointer*)voidptr;
                        if (pointer) {
                            delete pointer->shape;
                            delete pointer->strides;
                            Py_XDECREF(pointer->descr);
                            delete pointer;
                        }
                    };
                }
                
                PyObject* __array_interface__() const {
                    using imageref_t = typename shared_image_t::element_type const&;
                    shared_image_t strong;
                    char const* structcode;
                    std::string dsig;
                    long literal_pointer;
                    
                    {
                        py::gil::release nogil;
                        strong = image.lock();
                        structcode = strong->structcode();
                        dsig = strong->dsignature();
                        literal_pointer = (long)strong->data();
                    }
                    
                    PyObject* map = PyDict_New();
                    imageref_t imageref = *strong.get();
                    py::detail::setitemstring(map, "version",    py::convert(3));
                    py::detail::setitemstring(map, "shape",      py::detail::image_shape(imageref));
                    py::detail::setitemstring(map, "strides",    py::detail::image_strides(imageref));
                    py::detail::setitemstring(map, "descr",      py::detail::structcode_to_dtype(structcode));
                    py::detail::setitemstring(map, "mask",       py::None());
                    py::detail::setitemstring(map, "offset",     py::None());
                    py::detail::setitemstring(map, "data",       py::tuple(PyLong_FromLong(literal_pointer), py::True()));
                    py::detail::setitemstring(map, "typestr",    py::string(dsig));
                    return map;
                }
                
                PyObject* __array_struct__() const {
                    PyArrayInterface* newstruct;
                    {
                        py::gil::release nogil;
                        shared_image_t strong = image.lock();
                        newstruct = py::numpy::array_struct(*strong.get());
                    }
                    
                    return py::cob::objectify<PyArrayInterface, py::cob::single_destructor_t>(
                                              newstruct,
                                              array_destructor<PyArrayInterface>());
                }
            
                static char const* typestring() {
                    static FactoryType factory;
                    static std::string name = "im." + factory.name() + ".Buffer";
                    return name.c_str();
                }
                
                static char const* typedoc()    { return "Python image-backed buffer class\n"; }
                
            }; /* BufferModel */
            
            void* operator new(std::size_t newsize) {
                PyTypeObject* type = FactoryType::image_type();
                return reinterpret_cast<void*>(type->tp_alloc(type, 0));
            }
            
            void operator delete(void* voidself) {
                ImageModelBase* self = reinterpret_cast<ImageModelBase*>(voidself);
                PyObject* pyself = reinterpret_cast<PyObject*>(voidself);
                if (self->weakrefs != nullptr) { PyObject_ClearWeakRefs(pyself); }
                self->cleanup();
                FactoryType::image_type()->tp_free(pyself);
            }
            
            struct Tag {
                struct FromImage            {};
                struct FromBuffer           {};
                struct FromImageBuffer      {};
                struct FromOtherImageBuffer {};
                struct FromImagePlane       {};
            };
            
            PyObject_HEAD
            PyObject* weakrefs = nullptr;
            shared_image_t image;
            PyArray_Descr* dtype = nullptr;
            PyObject* imagebuffer = nullptr;
            PyObject* readoptDict = nullptr;
            PyObject* writeoptDict = nullptr;
            bool clean = false;
            
            ImageModelBase()
                :weakrefs(nullptr)
                ,image(std::make_shared<ImageType>())
                ,dtype(nullptr)
                ,imagebuffer(nullptr)
                ,readoptDict(nullptr)
                ,writeoptDict(nullptr)
                {}
            
            ImageModelBase(ImageModelBase const& other)
                :weakrefs(nullptr)
                ,image(other.image)
                ,dtype(PyArray_DescrFromType(image->dtype()))
                ,imagebuffer(reinterpret_cast<PyObject*>(
                             new typename ImageModelBase::BufferModel(image)))
                ,readoptDict(PyDict_New())
                ,writeoptDict(PyDict_New())
                {
                    Py_INCREF(dtype);
                    Py_INCREF(imagebuffer);
                    Py_INCREF(readoptDict);
                    Py_INCREF(writeoptDict);
                    PyDict_Update(readoptDict,  other.readoptDict);
                    PyDict_Update(writeoptDict, other.writeoptDict);
                }
            
            explicit ImageModelBase(ImageModelBase const& other, int zidx)
                :weakrefs(nullptr)
                ,image(std::make_shared<ImageType>(*other.image.get(), zidx))
                ,dtype(PyArray_DescrFromType(image->dtype()))
                ,imagebuffer(reinterpret_cast<PyObject*>(
                             new typename ImageModelBase::BufferModel(image)))
                ,readoptDict(PyDict_New())
                ,writeoptDict(PyDict_New())
                {
                    Py_INCREF(dtype);
                    Py_INCREF(imagebuffer);
                    Py_INCREF(readoptDict);
                    Py_INCREF(writeoptDict);
                }
            
            explicit ImageModelBase(ImageModelBase const& basis,
                                    ImageModelBase const& etc)
                :weakrefs(nullptr)
                ,image(std::make_shared<ImageType>(*basis.image.get(),
                                                     *etc.image.get()))
                ,dtype(PyArray_DescrFromType(image->dtype()))
                ,imagebuffer(reinterpret_cast<PyObject*>(
                             new typename ImageModelBase::BufferModel(image)))
                ,readoptDict(PyDict_New())
                ,writeoptDict(PyDict_New())
                {
                    Py_INCREF(dtype);
                    Py_INCREF(imagebuffer);
                    Py_INCREF(readoptDict);
                    Py_INCREF(writeoptDict);
                }
            
            ImageModelBase(ImageModelBase&& other) noexcept
                :weakrefs(other.weakrefs)
                ,image(std::move(other.image))
                ,dtype(other.dtype)
                ,imagebuffer(other.imagebuffer)
                ,readoptDict(other.readoptDict)
                ,writeoptDict(other.writeoptDict)
                {
                    other.weakrefs = nullptr;
                    other.image.reset(nullptr);
                    other.dtype = nullptr;
                    other.imagebuffer = nullptr;
                    other.readoptDict = nullptr;
                    other.writeoptDict = nullptr;
                    other.clean = true;
                }
            
            /// reinterpret, depointerize, copy-construct
            explicit ImageModelBase(PyObject* other, typename Tag::FromImage tag = typename Tag::FromImage{})
                :ImageModelBase(*reinterpret_cast<ImageModelBase*>(other))
                {}
            
            /// bmoi = Buffer Model Object Instance
            explicit ImageModelBase(BufferModelBase<BufferType> const& bmoi)
                :weakrefs(nullptr)
                ,image(std::make_shared<ImageType>(NPY_UINT8, bmoi.internal.get()))
                ,dtype(PyArray_DescrFromType(image->dtype()))
                ,imagebuffer(reinterpret_cast<PyObject*>(
                             new typename ImageModelBase::BufferModel(image)))
                ,readoptDict(PyDict_New())
                ,writeoptDict(PyDict_New())
                ,clean(false)
                {
                    Py_INCREF(dtype);
                    Py_INCREF(imagebuffer);
                    Py_INCREF(readoptDict);
                    Py_INCREF(writeoptDict);
                }
            
            /// tag dispatch, reinterpret, depointerize, explicit-init-style construct
            explicit ImageModelBase(PyObject* buffer, typename Tag::FromBuffer)
                :ImageModelBase(*reinterpret_cast<BufferModelBase<BufferType>*>(buffer))
                {}
            
            /// tag dispatch, reinterpret, depointerize, explicit-init-style construct
            explicit ImageModelBase(PyObject* source, int zidx, typename Tag::FromImagePlane = typename Tag::FromImagePlane{})
                :ImageModelBase(*reinterpret_cast<ImageModelBase*>(source), zidx)
                {}
            
            explicit ImageModelBase(PyObject* basis, PyObject* etc)
                :ImageModelBase(*reinterpret_cast<ImageModelBase*>(basis),
                                *reinterpret_cast<ImageModelBase*>(etc))
                {
                    Py_DECREF(basis);
                }
            
            explicit ImageModelBase(int width, int height,
                                    int planes = 1,
                                    int value = 0x00,
                                    int nbits = 8, bool is_signed = false)
                :weakrefs(nullptr)
                ,image(std::make_shared<ImageType>(
                       im::detail::for_nbits(nbits, is_signed),
                                             width, height,
                                             planes > 0 ? planes : 1))
                ,dtype(PyArray_DescrFromType(image->dtype()))
                ,imagebuffer(reinterpret_cast<PyObject*>(
                             new typename ImageModelBase::BufferModel(image)))
                ,readoptDict(PyDict_New())
                ,writeoptDict(PyDict_New())
                ,clean(false)
                {
                    Py_INCREF(dtype);
                    Py_INCREF(imagebuffer);
                    Py_INCREF(readoptDict);
                    Py_INCREF(writeoptDict);
                    if (value > -1) {
                        py::gil::release nogil;
                        std::memset(image->rowp(0), value,
                                    image->size() * dtype->elsize);
                    }
                }
            
            ImageModelBase& operator=(ImageModelBase const& other) {
                if (&other != this) {
                    ImageModelBase(other).swap(*this);
                }
                return *this;
            }
            ImageModelBase& operator=(ImageModelBase&& other) noexcept {
                if (&other != this) {
                    weakrefs = other.weakrefs;
                    image = std::move(other.image);
                    dtype = other.dtype;
                    imagebuffer = other.imagebuffer;
                    readoptDict = other.readoptDict;
                    writeoptDict = other.writeoptDict;
                    other.weakrefs = nullptr;
                    other.image.reset(nullptr);
                    other.dtype = nullptr;
                    other.imagebuffer = nullptr;
                    other.readoptDict = nullptr;
                    other.writeoptDict = nullptr;
                    other.clean = true;
                }
                return *this;
            }
            ImageModelBase& operator=(PyObject* other) {
                if (other != this) {
                    ImageModelBase(*reinterpret_cast<ImageModelBase*>(other)).swap(*this);
                }
                return *this;
            }
            
            void swap(ImageModelBase& other) noexcept {
                using std::swap;
                swap(weakrefs,      other.weakrefs);
                swap(image,         other.image);
                swap(dtype,         other.dtype);
                swap(imagebuffer,   other.imagebuffer);
                swap(readoptDict,   other.readoptDict);
                swap(writeoptDict,  other.writeoptDict);
            }
            
            void cleanup(bool force = false) {
                if (!clean || force) {
                    Py_CLEAR(dtype);
                    Py_CLEAR(imagebuffer);
                    Py_CLEAR(readoptDict);
                    Py_CLEAR(writeoptDict);
                    clean = !force;
                }
            }
            
            /// Arguments to ImageModel::vacay() are as required
            /// by the Py_VISIT(), w/r/t both types and names
            int vacay(visitproc visit, void* arg) {
                Py_VISIT(dtype);
                Py_VISIT(readoptDict);
                Py_VISIT(writeoptDict);
                return 0;
            }
            
            long __hash__() {
                long out;
                {
                    py::gil::release nogil;
                    auto bithash = blockhash::blockhash_quick(*image);
                    out = static_cast<long>(bithash.to_ulong());
                }
                return out;
            }
            
            options_map readopts() {
                return py::options::parse(readoptDict);
            }
            
            options_map writeopts() {
                return py::options::parse(writeoptDict);
            }
            
            bool load(char const* source, options_map const& opts) {
                FactoryType factory;
                std::unique_ptr<ImageFormat> format;
                std::unique_ptr<im::FileSource> input;
                std::unique_ptr<Image> output;
                bool exists = false,
                     can_read = false;
                options_map default_opts;
                
                try {
                    py::gil::release nogil;
                    format = im::for_filename(source);
                    can_read = format->format_can_read();
                } catch (im::FormatNotFound& exc) {
                    PyErr_Format(PyExc_ValueError,
                        "Can't find I/O format for file: %s", source);
                    return false;
                }
                
                if (!can_read) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented read() in I/O format %s",
                        mime.c_str());
                    return false;
                }
                
                {
                    py::gil::release nogil;
                    input = std::make_unique<im::FileSource>(source);
                    exists = input->exists();
                }
                
                if (!exists) {
                    PyErr_Format(PyExc_ValueError,
                        "Can't find image file: %s", source);
                    return false;
                }
                
                {
                    py::gil::release nogil;
                    default_opts = format->add_options(opts);
                    output = format->read(input.get(), &factory, default_opts);
                    image.reset(dynamic_cast<ImageType*>(output.release()));
                }
                
                return true;
            }
            
            bool load(Py_buffer const& view, options_map const& opts) {
                py::buffer::source source(view);
                return load(source.str().c_str(), opts);
            }
            
            bool loadfilelike(PyObject* file, options_map const& opts) {
                FactoryType factory;
                std::unique_ptr<ImageFormat> format;
                typename py::gil::with::source_t input;
                std::unique_ptr<Image> output;
                std::string suffix;
                options_map default_opts;
                bool can_read = false;
                
                try {
                    py::gil::with iohandle(file);
                    input = iohandle.source();
                    format = im::for_source(input.get());
                    can_read = format->format_can_read();
                    suffix = format->get_suffix();
                    if (can_read) {
                        default_opts = format->add_options(opts);
                        output = format->read(input.get(), &factory, default_opts);
                        image.reset(dynamic_cast<ImageType*>(output.release()));
                        return true;
                    }
                } catch (im::FormatNotFound& exc) {
                    PyErr_SetString(PyExc_ValueError,
                        "Can't match blob data to a suitable I/O format");
                    return false;
                }
                
                if (format.get()) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented read() in I/O format %s",
                        mime.c_str());
                } else {
                    PyErr_SetString(PyExc_ValueError,
                        "Bad I/O format pointer returned for blob data");
                }
                return false;
            }
            
            bool loadblob(Py_buffer const& view, options_map const& opts) {
                FactoryType factory;
                std::unique_ptr<ImageFormat> format;
                std::unique_ptr<py::buffer::source> input;
                std::unique_ptr<Image> output;
                options_map default_opts;
                bool can_read = false;
                
                try {
                    py::gil::release nogil;
                    input = std::make_unique<py::buffer::source>(view);
                    format = im::for_source(input.get());
                    can_read = format->format_can_read();
                    if (can_read) {
                        default_opts = format->add_options(opts);
                        output = format->read(input.get(), &factory, default_opts);
                        image.reset(dynamic_cast<ImageType*>(output.release()));
                        return true;
                    }
                } catch (im::FormatNotFound& exc) {
                    PyErr_SetString(PyExc_ValueError,
                        "Can't match blob data to a suitable I/O format");
                    return false;
                }
                
                if (format.get()) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented read() in I/O format %s",
                        mime.c_str());
                } else {
                    PyErr_SetString(PyExc_ValueError,
                        "Bad I/O format pointer returned for blob data");
                }
                return false;
            }
            
            bool save(char const* destination, options_map const& opts) {
                std::unique_ptr<ImageFormat> format;
                options_map default_opts;
                bool exists = false,
                     can_write = false,
                     overwrite = true;
                
                try {
                    py::gil::release nogil;
                    format = im::for_filename(destination);
                    can_write = format->format_can_write();
                } catch (im::FormatNotFound& exc) {
                    PyErr_Format(PyExc_ValueError,
                        "Can't find I/O format for file: %s", destination);
                    return false;
                }
                
                if (!can_write) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented write() in I/O format %s",
                        mime.c_str());
                    return false;
                }
                
                {
                    py::gil::release nogil;
                    exists = path::exists(destination);
                    overwrite = opts.cast<bool>("overwrite", true);
                }
                
                if (exists && !overwrite) {
                    PyErr_Format(PyExc_ValueError,
                        "File exists (opts['overwrite'] == False): %s",
                        destination);
                    return false;
                }
                {
                    py::gil::release nogil;
                    if (exists && overwrite) {
                        path::remove(destination);
                    }
                    std::unique_ptr<im::FileSink> output(new im::FileSink(destination));
                    default_opts = format->add_options(opts);
                    format->write(dynamic_cast<Image&>(*image.get()),
                                                       output.get(), default_opts);
                }
                
                return true;
            }
            
            bool savefilelike(PyObject* file, options_map const& opts) {
                std::unique_ptr<ImageFormat> format;
                typename py::gil::with::sink_t output;
                std::string ext;
                options_map default_opts;
                bool can_write = false;
                
                if (!opts.has("format")) {
                    PyErr_SetString(PyExc_AttributeError,
                        "Output format unspecified in options dict");
                    return false;
                }
                
                try {
                    py::gil::with iohandle(file);
                    output = iohandle.sink();
                    ext = opts.cast<std::string>("format");
                    format = im::get_format(ext.c_str());
                    can_write = format->format_can_write();
                } catch (im::FormatNotFound& exc) {
                    PyErr_Format(PyExc_ValueError,
                        "Can't find I/O format: %s",
                        opts.cast<char const*>("format"));
                    return false;
                }
                
                if (!can_write) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented write() in I/O format %s",
                        mime.c_str());
                    return false;
                }
                
                {
                    py::gil::release nogil;
                    default_opts = format->add_options(opts);
                    format->write(dynamic_cast<Image&>(*image.get()),
                                                       output.get(), default_opts);
                }
                
                return true;
            }
            
            PyObject* saveblob(options_map const& opts) {
                std::unique_ptr<ImageFormat> format;
                std::unique_ptr<im::FileSink> output;
                std::unique_ptr<im::FileSource> readback;
                std::vector<byte> data;
                std::string ext;
                options_map default_opts;
                bool can_write = false,
                     removed = false,
                     as_url = false,
                     as_html = false;
                
                if (!opts.has("format")) {
                    PyErr_SetString(PyExc_AttributeError,
                        "Output format unspecified in options dict");
                    return nullptr;
                }
                
                try {
                    py::gil::release nogil;
                    ext = opts.cast<std::string>("format");
                    format = im::get_format(ext.c_str());
                    can_write = format->format_can_write();
                } catch (im::FormatNotFound& exc) {
                    PyErr_Format(PyExc_ValueError,
                        "Can't find I/O format: %s",
                        ext.c_str());
                    return nullptr;
                }
                
                if (!can_write) {
                    std::string mime = format->get_mimetype();
                    PyErr_Format(PyExc_ValueError,
                        "Unimplemented write() in I/O format %s",
                        mime.c_str());
                    return nullptr;
                }
                
                {
                    py::gil::release nogil;
                    NamedTemporaryFile tf(format->get_suffix(true),
                                          FILESYSTEM_TEMP_FILENAME,
                                          false);
                    
                    std::string pth = tf.filepath.make_absolute().str();
                    tf.filepath.remove();
                    output = std::make_unique<im::FileSink>(pth.c_str());
                    
                    default_opts = format->add_options(opts);
                    format->write(dynamic_cast<Image&>(*image.get()),
                                                       output.get(), default_opts);
                    output->flush();
                    
                    if (!path::exists(pth)) {
                        py::gil::ensure yesgil;
                        PyErr_SetString(PyExc_ValueError,
                            "Temporary file is AWOL");
                        return nullptr;
                    }
                    
                    readback = std::make_unique<im::FileSource>(pth.c_str());
                    data = readback->full_data();
                    readback->close();
                    readback.reset(nullptr);
                    tf.close();
                    removed = tf.remove();
                }
                
                if (!removed) {
                    PyErr_SetString(PyExc_ValueError,
                        "Failed to remove temporary file");
                    return nullptr;
                }
                
                as_html = opts.cast<bool>("as_html", false);
                as_url = opts.cast<bool>("as_url", as_html);
                if (!as_url) { return py::string(data); }
                
                std::string out("data:");
                {
                    py::gil::release nogil;
                    if (as_url) {
                        out += format->get_mimetype() + ";base64,";
                        out += im::base64::encode(&data[0], data.size());
                        if (as_html) {
                            out = std::string("<img src='") + out + "'>";
                        }
                    }
                }
                return py::string(out);
            }
            
            static constexpr Py_ssize_t typeflags() {
                return Py_TPFLAGS_DEFAULT         |
                       Py_TPFLAGS_BASETYPE        |
                       Py_TPFLAGS_HAVE_GC         |
                       Py_TPFLAGS_HAVE_WEAKREFS   |
                       Py_TPFLAGS_HAVE_NEWBUFFER;
            }
            
            static char const* typestring() {
                static FactoryType factory;
                static std::string name = "im." + factory.name();
                return name.c_str();
            }
            
            static char const* typedoc()    { return "Buffered-image multibackend model base class\n"; }
            
        }; /* ImageModelBase */
        
    } /* namespace ext */
    
}

#endif /// LIBIMREAD_PYTHON_IM_INCLUDE_MODELS_IMAGEMODEL_HH_