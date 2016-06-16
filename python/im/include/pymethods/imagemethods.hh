
#ifndef LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_IMAGEMETHODS_HH_
#define LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_IMAGEMETHODS_HH_

#include <cmath>
#include <memory>
#include <string>
#include <Python.h>
#include <structmember.h>

#include "../buffer.hpp"
#include "../check.hh"
#include "../gil.hpp"
#include "../gil-io.hpp"
#include "../detail.hpp"
#include "../options.hpp"
#include "../pybuffer.hpp"

#include <libimread/ext/errors/demangle.hh>
#include <libimread/ext/filesystem/path.h>
#include <libimread/ext/filesystem/temporary.h>
#include <libimread/hashing.hh>
#include <libimread/pixels.hh>

#include "../models/models.hh"

namespace py {
    
    namespace ext {
        
        namespace image {
            
            /// ALLOCATE / __new__ implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* createnew(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
                // using tag_t = typename PythonImageType::Tag::FromImage;
                return py::convert(new PythonImageType());
            }
            
            /// ALLOCATE / new(width, height, planes, fill, nbits, is_signed) implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* newfromsize(PyObject* _nothing_, PyObject* args, PyObject* kwargs) {
                PyObject* py_is_signed = nullptr;
                PyObject* py_fill = nullptr;
                bool is_signed = false;
                uint32_t unbits = 8;
                int width   = -1,
                    height  = -1,
                    planes  = 1,
                    fill    = 0x00,
                    nbits   = 8;
                char const* keywords[] = { "width", "height", "planes",
                                           "fill",  "nbits",  "is_signed", nullptr };
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "ii|iOIO:new", const_cast<char**>(keywords),
                    &width,                 /// "width", int, HOW WIDE <required>
                    &height,                /// "height", int, HOW HIGH <required>
                    &planes,                /// "planes", int, # of channels
                    &py_fill,               /// "fill", Python object providing fill values (int or callable)
                    &unbits,                /// "nbits", unsigned int, # of bits
                    &py_is_signed))         /// "is_signed", Python boolean, value-signedness
                {
                    return nullptr;
                }
                
                is_signed = py::options::truth(py_is_signed);
                
                switch (unbits) {
                    case 1:
                    case 8:
                    case 16:
                    case 32:
                    case 64:
                        nbits = static_cast<int>(unbits);
                        break;
                    default:
                        PyErr_Format(PyExc_ValueError,
                            "bad nbits value: %u (must be 1, 8, 16, 32, or 64)",
                            unbits);
                        return nullptr;
                }
                
                if (py_fill) {
                    if (PyCallable_Check(py_fill)) {
                        /// DO ALL SORTS OF SHIT HERE WITH PyObject_CallObject
                        PyErr_SetString(PyExc_NotImplementedError,
                            "callable filling not implemented");
                        return nullptr;
                    } else {
                        /// We can't use the commented-out line below,
                        /// because 'fill' is currently implemented with
                        /// std::memset() in the constructor we call below.
                        /// Even though std::memset()'s value arg is an int,
                        /// it internally converts it to an unsigned char
                        /// before setting the mem. 
                        if (PyLong_Check(py_fill) || PyInt_Check(py_fill)) {
                            Py_ssize_t fillval = PyInt_AsSsize_t(py_fill);
                            // fill = detail::clamp(fillval, 0L, std::pow(2L, unbits));
                            fill = detail::clamp(fillval, 0L, 255L);
                        }
                    }
                }
                
                /// create new PyObject* image model instance
                PyObject* self = py::convert(
                    new PythonImageType(width, height, planes,
                                        fill,  nbits,  is_signed));
                
                /// return the new instance
                return self;
            }
            
            
            /// ALLOCATE / frombuffer(bufferInstance) implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* newfrombuffer(PyObject* _nothing_, PyObject* buffer) {
                using tag_t = typename PythonImageType::Tag::FromBuffer;
                if (!buffer) {
                    PyErr_SetString(PyExc_ValueError,
                        "missing im.Buffer argument");
                    return nullptr;
                }
                if (!BufferModel_Check(buffer) &&
                    !ImageBufferModel_Check(buffer) &&
                    !ArrayBufferModel_Check(buffer)) {
                    PyErr_SetString(PyExc_ValueError,
                        "invalid im.Buffer instance");
                    return nullptr;
                }
                return py::convert(new PythonImageType(buffer, tag_t{}));
            }
            
            /// ALLOCATE / fromimage(imageInstance) implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* newfromimage(PyObject* _nothing_, PyObject* other) {
                using tag_t = typename PythonImageType::Tag::FromImage;
                if (!other) {
                    PyErr_SetString(PyExc_ValueError,
                        "missing ImageType argument");
                    return nullptr;
                }
                if (!ImageModel_Check(other) &&
                    !ArrayModel_Check(other)) {
                    PyErr_SetString(PyExc_ValueError,
                        "invalid ImageType instance");
                    return nullptr;
                }
                return py::convert(new PythonImageType(other, tag_t{}));
            }
            
            /// ALLOCATE / merge(tuple(imageInstance, imageInstance [...])) implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* newfrommerge(PyTypeObject* type, PyObject* planes) {
                /// check our argument
                if (!PySequence_Check(planes)) {
                    PyErr_SetString(PyExc_ValueError,
                        "invalid ImageType sequence");
                    return nullptr;
                }
                
                /// set up fast-sequence iterable
                PyObject* basis = nullptr;
                PyObject* sequence = PySequence_Fast(planes, "Sequence expected");
                int idx = 0,
                    len = PySequence_Fast_GET_SIZE(sequence);
                
                if (len < 1) {
                    Py_DECREF(sequence);
                    PyErr_SetString(PyExc_ValueError,
                        "Sequence has no items");
                    return nullptr;
                }
                
                /// check the sequences' type (essentially against `type(self)`)
                PyObject* pynitial = PySequence_Fast_GET_ITEM(sequence, idx);
                if (!PyObject_TypeCheck(pynitial, type)) {
                    Py_DECREF(sequence);
                    PyErr_SetString(PyExc_TypeError,
                        "Wrong sequence item type");
                    return nullptr;
                }
                
                /// store the initial sequence items' dimensions
                PythonImageType* initial = reinterpret_cast<PythonImageType*>(pynitial);
                int width = initial->image->dim(0),
                    height = initial->image->dim(1);
                
                /// loop and check all sequence items a) for correct type
                /// and b) that their dimensions match those of the initial item
                for (idx = 1; idx < len; idx++) {
                    PythonImageType* item = reinterpret_cast<PythonImageType*>(
                                            PySequence_Fast_GET_ITEM(sequence, idx));
                    if (!PyObject_TypeCheck(py::convert(item), type)) {
                        Py_DECREF(sequence);
                        PyErr_SetString(PyExc_TypeError,
                            "Mismatched image type");
                        return nullptr;
                    }
                    if (item->image->dim(0) != width ||
                        item->image->dim(1) != height) {
                        Py_DECREF(sequence);
                        PyErr_SetString(PyExc_AttributeError,
                            "Mismatched image dimensions");
                        return nullptr;
                    }
                }
                
                /// actual allocation loop
                basis = PySequence_Fast_GET_ITEM(sequence, 0);
                Py_INCREF(basis);
                if (len > 1) {
                    for (idx = 1; idx < len; idx++) {
                        basis = py::convert(
                                new PythonImageType(basis,
                                    PySequence_Fast_GET_ITEM(sequence, idx)));
                    }
                }
                
                /// clean up fast-sequence iterable before returning
                Py_DECREF(sequence);
                return basis;
            }
            
            /// __init__ implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            int init(PyObject* self, PyObject* args, PyObject* kwargs) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                PyObject* py_is_blob = nullptr;
                PyObject* options = nullptr;
                PyObject* file = nullptr;
                Py_buffer view;
                options_map opts;
                char const* keywords[] = { "source", "file", "is_blob", "options", nullptr };
                bool is_blob = false;
                bool did_load = false;
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "|s*OOO:__init__", const_cast<char**>(keywords),
                    &view,                      /// "view", buffer with file path or image data
                    &file,                      /// "file", possible file-like object
                    &py_is_blob,                /// "is_blob", Python boolean specifying blobbiness
                    &options))                  /// "options", read-options dict
                {
                    return -1;
                }
                
                /// test is necessary, the next line chokes on nullptr:
                is_blob = py::options::truth(py_is_blob);
                opts = py::options::parse(options);
                
                if (file) {
                    /// load as file-like Python object
                    did_load = pyim->loadfilelike(file, opts);
                } else if (is_blob) {
                    /// load as blob -- pass the buffer along
                    did_load = pyim->loadblob(view, opts);
                } else {
                    /// load as file -- pass the buffer along
                    did_load = pyim->load(view, opts);
                }
                
                if (!did_load) {
                    /// If this is true, PyErr has already been set
                    PyErr_SetString(PyExc_IOError,
                        "Image binary load failed");
                        return -1;
                }
                
                /// create and set a dtype based on the loaded image data's type
                Py_CLEAR(pyim->dtype);
                pyim->dtype = PyArray_DescrFromType(pyim->image->dtype());
                
                /// allocate a new image buffer
                Py_CLEAR(pyim->imagebuffer);
                pyim->imagebuffer = py::convert(new imagebuffer_t(pyim->image));
                
                /// store the read options dict
                Py_CLEAR(pyim->readoptDict);
                pyim->readoptDict = options ? py::object(options) : PyDict_New();
                
                /// ... and now OK, store an empty write options dict
                Py_CLEAR(pyim->writeoptDict);
                pyim->writeoptDict = PyDict_New();
                
                /// ALL IS WELL:
                return 0;
            }
            
            /// __repr__ implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* repr(PyObject* self) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                static bool named = false;
                static char const* pytypename;
                if (!named) {
                    py::gil::release nogil;
                    pytypename = terminator::nameof(pyim);
                    named = true;
                }
                return PyString_FromFormat(
                    "< %s @ %p >",
                    pytypename, pyim);
            }
            
            /// __str__ implementaton -- return bytes of image
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* str(PyObject* self) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                char const* string_ptr;
                Py_ssize_t string_size;
                {
                    py::gil::release nogil;
                    string_ptr = pyim->image->template rowp_as<char const>(0);
                    string_size = pyim->image->size();
                }
                return py::string(string_ptr, string_size);
            }
            
            /// __hash__ implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            long hash(PyObject* self) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return pyim->__hash__();
            }
            
            /// __len__ implementation
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            Py_ssize_t length(PyObject* self) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                imagebuffer_t* imbuf = reinterpret_cast<imagebuffer_t*>(pyim->imagebuffer);
                return imbuf->__len__();
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* atindex(PyObject* self, Py_ssize_t idx) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                imagebuffer_t* imbuf = reinterpret_cast<imagebuffer_t*>(pyim->imagebuffer);
                return imbuf->__index__(idx, static_cast<int>(pyim->dtype->type_num));
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            int getbuffer(PyObject* self, Py_buffer* view, int flags) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                imagebuffer_t* imbuf = reinterpret_cast<imagebuffer_t*>(pyim->imagebuffer);
                return imbuf->getbuffer(view, flags, static_cast<NPY_TYPES>(pyim->dtype->type_num));
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            void releasebuffer(PyObject* self, Py_buffer* view) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                imagebuffer_t* imbuf = reinterpret_cast<imagebuffer_t*>(pyim->imagebuffer);
                imbuf->releasebuffer(view);
            }
            
            /// DEALLOCATE
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            void dealloc(PyObject* self) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                delete pyim;
            }
            
            /// CLEAR
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            int clear(PyObject* self) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                pyim->cleanup(true);
                return 0;
            }
            
            /// TRAVERSE
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            int traverse(PyObject* self, visitproc visit, void* arg) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                pyim->vacay(visit, arg);
                return 0;
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* write(PyObject* self, PyObject* args, PyObject* kwargs) {
                using iosource_t = typename py::gil::with::source_t;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                PyObject* py_as_blob = nullptr;
                PyObject* options = nullptr;
                PyObject* file = nullptr;
                Py_buffer view;
                char const* keywords[] = { "destination", "file", "as_blob", "options", nullptr };
                std::string dststr;
                bool as_blob = false,
                     use_file = false,
                     did_save = false;
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "|s*OOO:write", const_cast<char**>(keywords),
                    &view,                      /// "destination", buffer with file path
                    &file,                      /// "file", PyFileObject*-castable I/O handle
                    &py_as_blob,                /// "as_blob", Python boolean specifying blobbiness
                    &options))                  /// "options", read-options dict
                {
                    return nullptr;
                }
                
                /// tests are necessary, the next lines choke on nullptr:
                as_blob = py::options::truth(py_as_blob);
                if (file) { use_file = PyFile_Check(file); }
                if (options == nullptr) { options = PyDict_New(); }
                
                if (PyDict_Update(pyim->writeoptDict, options) == -1) {
                    Py_DECREF(options);
                    PyErr_SetString(PyExc_SystemError,
                        "Dictionary update failure");
                    return nullptr;
                }
                
                options_map opts = pyim->writeopts();
                Py_DECREF(options);
                
                if (as_blob || use_file) {
                    if (!opts.has("format")) {
                        PyErr_SetString(PyExc_AttributeError,
                            "Output format unspecified");
                        return nullptr;
                    }
                }
                
                if (!use_file) {
                    if (as_blob) {
                        py::gil::release nogil;
                        NamedTemporaryFile tf("." + opts.cast<std::string>("format"),
                                            FILESYSTEM_TEMP_FILENAME, false); /// boolean cleanup on scope exit
                        dststr = std::string(tf.filepath.make_absolute().str());
                    } else {
                        /// save as file -- extract the filename from the buffer
                        py::gil::release nogil;
                        py::buffer::source dest(view);
                        dststr = std::string(dest.str());
                    }
                    if (!dststr.size()) {
                        if (as_blob) {
                            PyErr_SetString(PyExc_ValueError,
                                "Blob output unexpectedly returned zero-length bytestring");
                        } else {
                            PyErr_SetString(PyExc_ValueError,
                                "File output destination path is unexpectedly zero-length");
                        }
                        return nullptr;
                    }
                    did_save = pyim->save(dststr.c_str(), opts);
                } else {
                    did_save = pyim->savefilelike(file, opts);
                }
                
                if (!did_save) {
                    return nullptr; /// If this is false, PyErr has been set
                }
                
                if (as_blob) {
                    std::vector<byte> data;
                    if (use_file) {
                        py::gil::with iohandle(file);
                        iosource_t readback = iohandle.source();
                        data = readback->full_data();
                    } else {
                        bool removed = false;
                        {
                            py::gil::release nogil;
                            std::unique_ptr<FileSource> readback(
                                new FileSource(dststr.c_str()));
                            data = readback->full_data();
                            readback->close();
                            removed = path::remove(dststr);
                        }
                        if (!removed) {
                            PyErr_Format(PyExc_IOError,
                                "Failed to remove temporary file %s",
                                dststr.c_str());
                            return nullptr;
                        }
                    }
                    return py::string(data);
                }
                /// "else":
                if (use_file) { return py::None(); }
                return py::string(dststr);
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* split(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                const int planes = pyim->image->planes();
                if (planes > 1) {
                    PyObject* out = PyTuple_New(planes);
                    for (int idx = 0; idx < planes; ++idx) {
                        PyTuple_SET_ITEM(out, idx, py::convert(new PythonImageType(self, idx)));
                    }
                    return out;
                }
                return py::tuplize(new PythonImageType(self, 0));
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* plane_at(PyObject* self, PyObject* args, PyObject* kwargs) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                std::string::size_type midx;
                int idx = -1;
                char const* planeptr = nullptr;
                char const* keywords[] = { "index", "plane", nullptr };
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "|Is:plane_at", const_cast<char**>(keywords),
                    &idx,                   /// "index", uint, WHICH PLANE (numerically)
                    &planeptr))             /// "plane", string, WHICH PLANE (lexically)
                {
                    return nullptr;
                }
                
                if (idx == -1 && planeptr == nullptr) {
                    PyErr_SetString(PyExc_ValueError,
                        "index (unsigned int) or plane (string) required");
                    return nullptr;
                } else if (idx > 0 && planeptr != nullptr) {
                    PyErr_SetString(PyExc_ValueError,
                        "specify index (unsigned int) or plane (string) but not both");
                    return nullptr;
                }
                
                if (planeptr) {
                    std::string mode = pyim->modestring();
                    midx = mode.find(planeptr[0]);
                    if (midx == std::string::npos) {
                        /// not found
                        PyErr_Format(PyExc_ValueError,
                            "plane '%c' not found in mode '%s'",
                            planeptr[0], mode.c_str());
                        return nullptr;
                    }
                    idx = static_cast<int>(midx);
                }
                
                return pyim->plane_at(idx);
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* jupyter_repr_png(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                PyObject* options = PyDict_New();
                py::detail::setitemstring(options, "format", py::string("png"));
                if (PyDict_Update(pyim->writeoptDict, options) == -1) {
                    Py_DECREF(options);
                    PyErr_SetString(PyExc_SystemError,
                        "Dictionary update failure");
                    return nullptr;
                }
                Py_DECREF(options);
                return pyim->saveblob(pyim->writeopts());
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* jupyter_repr_jpeg(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                PyObject* options = PyDict_New();
                py::detail::setitemstring(options, "format", py::string("jpg"));
                if (PyDict_Update(pyim->writeoptDict, options) == -1) {
                    Py_DECREF(options);
                    PyErr_SetString(PyExc_SystemError,
                        "Dictionary update failure");
                    return nullptr;
                }
                Py_DECREF(options);
                return pyim->saveblob(pyim->writeopts());
            }
            
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject* jupyter_repr_html(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                PyObject* options = PyDict_New();
                py::detail::setitemstring(options, "format", py::string("jpg"));
                py::detail::setitemstring(options, "as_html", py::True());
                if (PyDict_Update(pyim->writeoptDict, options) == -1) {
                    Py_DECREF(options);
                    PyErr_SetString(PyExc_SystemError,
                        "Dictionary update failure");
                    return nullptr;
                }
                Py_DECREF(options);
                return pyim->saveblob(pyim->writeopts());
            }
            
            #define DECLARE_CLOSURE(name) static char const* name = #name
            #define CHECK_CLOSURE(name) (char const*)closure == closures::name
            #define BIND_CLOSURE(name) (void*)py::ext::image::closures::name
            
            namespace closures {
                DECLARE_CLOSURE(DTYPE);
                DECLARE_CLOSURE(BUFFER);
                DECLARE_CLOSURE(SHAPE);
                DECLARE_CLOSURE(STRIDES);
                DECLARE_CLOSURE(WIDTH);
                DECLARE_CLOSURE(HEIGHT);
                DECLARE_CLOSURE(PLANES);
                DECLARE_CLOSURE(READ);
                DECLARE_CLOSURE(WRITE);
                DECLARE_CLOSURE(STRUCT);
                DECLARE_CLOSURE(INTERFACE);
                DECLARE_CLOSURE(MODE);
                DECLARE_CLOSURE(HAS_ALPHA);
                DECLARE_CLOSURE(BITS);
                DECLARE_CLOSURE(BYTES);
            }
            
            /// ImageType.{mode,has_alpha} getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_mode_property(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return CHECK_CLOSURE(MODE) ? pyim->mode() : pyim->has_alpha();
            }
            
            /// ImageType.{bits,bytes} getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_size_property(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::convert(CHECK_CLOSURE(BITS) ? pyim->image->nbits() :
                                                         pyim->image->nbytes());
            }
            
            /// ImageType.{dtype,buffer} getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_subobject(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::object(CHECK_CLOSURE(DTYPE) ? py::convert(pyim->dtype) :
                                                                     pyim->imagebuffer);
            }
            
            /// ImageType.{shape,strides} getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_liminal_tuple(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return CHECK_CLOSURE(STRIDES) ? py::detail::image_strides(*pyim->image.get()) :
                                                py::detail::image_shape(*pyim->image.get());
            }
            
            /// ImageType.{width,height,planes} getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_dimensional_attribute(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::detail::image_dimensional_attribute(*pyim->image.get(), CHECK_CLOSURE(WIDTH) ? 0 :
                                                                                   CHECK_CLOSURE(HEIGHT) ? 1 : 2);
            }
            
            /// ImageType.{read,write}_opts getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_opts(PyObject* self, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::object(CHECK_CLOSURE(READ) ? pyim->readoptDict : pyim->writeoptDict);
            }
            
            /// ImageType.{read,write}_opts setter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            int          set_opts(PyObject* self, PyObject* value, void* closure) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                if (!value) { return 0; }
                if (!PyMapping_Check(value)) {
                    PyErr_SetString(PyExc_AttributeError,
                        "opts value must be dict-ish");
                    return -1;
                }
                /// dispatch on closure tag
                if (CHECK_CLOSURE(READ)) {
                    Py_CLEAR(pyim->readoptDict);
                    pyim->readoptDict = py::object(value);
                } else {
                    Py_CLEAR(pyim->writeoptDict);
                    pyim->writeoptDict = py::object(value);
                }
                return 0;
            }
            
            /// ImageType.__array_{struct,interface}__ getter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    get_array_attribute(PyObject* self, void* closure) {
                using imagebuffer_t = typename PythonImageType::BufferModel;
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                imagebuffer_t* imbuf = reinterpret_cast<imagebuffer_t*>(pyim->imagebuffer);
                return CHECK_CLOSURE(STRUCT) ? imbuf->__array_struct__() : imbuf->__array_interface__();
            }
            
            /// ImageType.read_opts formatter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    format_read_opts(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::string(pyim->readopts().format());
            }
            
            /// ImageType.read_opts file-dumper
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    dump_read_opts(PyObject* self, PyObject* args, PyObject* kwargs) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::options::dump(self, args, kwargs, pyim->readopts());
            }
            
            /// ImageType.write_opts formatter
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    format_write_opts(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::string(pyim->writeopts().format());
            }
            
            /// ImageType.write_opts file-dumper
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    dump_write_opts(PyObject* self, PyObject* args, PyObject* kwargs) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return py::options::dump(self, args, kwargs, pyim->writeopts());
            }
            
            /// ImageType.add_alpha() method
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    add_alpha(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return pyim->add_alpha();
            }
            
            /// ImageType.remove_alpha() method
            template <typename ImageType = HalideNumpyImage,
                      typename BufferType = buffer_t,
                      typename PythonImageType = ImageModelBase<ImageType, BufferType>>
            PyObject*    remove_alpha(PyObject* self, PyObject*) {
                PythonImageType* pyim = reinterpret_cast<PythonImageType*>(self);
                return pyim->remove_alpha();
            }
            
            namespace methods {
                
                template <typename ImageType,
                          typename BufferType = buffer_t>
                PyBufferProcs* buffer() {
                    static PyBufferProcs buffermethods = {
                        0, 0, 0, 0,
                        (getbufferproc)py::ext::image::getbuffer<ImageType, BufferType>,
                        (releasebufferproc)py::ext::image::releasebuffer<ImageType, BufferType>,
                    };
                    return &buffermethods;
                }
                
                template <typename ImageType,
                          typename BufferType = buffer_t>
                PySequenceMethods* sequence() {
                    static PySequenceMethods sequencemethods = {
                        (lenfunc)py::ext::image::length<ImageType, BufferType>,
                        0, 0,
                        (ssizeargfunc)py::ext::image::atindex<ImageType, BufferType>,
                        0, 0, 0, 0
                    };
                    return &sequencemethods;
                }
                
                template <typename ImageType,
                          typename BufferType = buffer_t>
                PyGetSetDef* getset() {
                    static PyGetSetDef getsets[] = {
                        {
                            (char*)"__array_interface__",
                                (getter)py::ext::image::get_array_attribute<ImageType, BufferType>,
                                nullptr,
                                (char*)"NumPy array interface (Python API) -> dict\n",
                                BIND_CLOSURE(INTERFACE) },
                        {
                            (char*)"__array_struct__",
                                (getter)py::ext::image::get_array_attribute<ImageType, BufferType>,
                                nullptr,
                                (char*)"NumPy array struct (C-level API) -> PyCObject\n",
                                BIND_CLOSURE(STRUCT) },
                        {
                            (char*)"bits",
                                (getter)py::ext::image::get_size_property<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image element size (in bits) -> int\n",
                                BIND_CLOSURE(BITS) },
                        {
                            (char*)"bytes",
                                (getter)py::ext::image::get_size_property<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image element size (in bytes) -> int\n",
                                BIND_CLOSURE(BYTES) },
                        {
                            (char*)"mode",
                                (getter)py::ext::image::get_mode_property<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image mode -> str\n",
                                BIND_CLOSURE(MODE) },
                        {
                            (char*)"has_alpha",
                                (getter)py::ext::image::get_mode_property<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image does or does not have an alpha channel -> bool\n",
                                BIND_CLOSURE(HAS_ALPHA) },
                        {
                            (char*)"dtype",
                                (getter)py::ext::image::get_subobject<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image dtype -> numpy.dtype\n",
                                BIND_CLOSURE(DTYPE) },
                        {
                            (char*)"buffer",
                                (getter)py::ext::image::get_subobject<ImageType, BufferType>,
                                nullptr,
                                (char*)"Underlying data buffer accessor object -> im.Buffer\n",
                                BIND_CLOSURE(BUFFER) },
                        {
                            (char*)"shape",
                                (getter)py::ext::image::get_liminal_tuple<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image shape tuple -> (int, int, int)\n",
                                BIND_CLOSURE(SHAPE) },
                        {
                            (char*)"strides",
                                (getter)py::ext::image::get_liminal_tuple<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image strides tuple -> (int, int, int)\n",
                                BIND_CLOSURE(STRIDES) },
                        {
                            (char*)"width",
                                (getter)py::ext::image::get_dimensional_attribute<ImageType, BufferType>,
                                nullptr,
                                (char*)"Pixel width -> int\n",
                                BIND_CLOSURE(WIDTH) },
                        {
                            (char*)"height",
                                (getter)py::ext::image::get_dimensional_attribute<ImageType, BufferType>,
                                nullptr,
                                (char*)"Pixel height -> int\n",
                                BIND_CLOSURE(HEIGHT) },
                        {
                            (char*)"planes",
                                (getter)py::ext::image::get_dimensional_attribute<ImageType, BufferType>,
                                nullptr,
                                (char*)"Image color planes (channels) -> int\n",
                                BIND_CLOSURE(PLANES) },
                        {
                            (char*)"read_opts",
                                (getter)py::ext::image::get_opts<ImageType, BufferType>,
                                (setter)py::ext::image::set_opts<ImageType, BufferType>,
                                (char*)"Read options -> dict\n",
                                BIND_CLOSURE(READ) },
                        {
                            (char*)"write_opts",
                                (getter)py::ext::image::get_opts<ImageType, BufferType>,
                                (setter)py::ext::image::set_opts<ImageType, BufferType>,
                                (char*)"Write options -> dict\n",
                                BIND_CLOSURE(WRITE) },
                        { nullptr, nullptr, nullptr, nullptr, nullptr }
                    };
                    return getsets;
                }
                
                template <typename ImageType,
                          typename BufferType = buffer_t>
                PyMethodDef* basic() {
                    static PyMethodDef basics[] = {
                        {
                            "_repr_jpeg_",
                                (PyCFunction)py::ext::image::jupyter_repr_jpeg<ImageType, BufferType>,
                                METH_NOARGS,
                                "image._repr_jpeg_()\n"
                                "\t-> Return the image data in the JPEG format\n"
                                "\t-> This method is for use by ipython/jupyter\n" },
                        {
                            "_repr_png_",
                                (PyCFunction)py::ext::image::jupyter_repr_png<ImageType, BufferType>,
                                METH_NOARGS,
                                "image._repr_png_()\n"
                                "\t-> Return the image data in the PNG format\n"
                                "\t-> This method is for use by ipython/jupyter\n" },
                        {
                            "_repr_html_",
                                (PyCFunction)py::ext::image::jupyter_repr_html<ImageType, BufferType>,
                                METH_NOARGS,
                                "image._repr_html_()\n"
                                "\t-> Return the image data as a base64-encoded `data:` URL inside an HTML <img> tag\n"
                                "\t-> This method is for use by ipython/jupyter\n" },
                        {
                            "check",
                                (PyCFunction)py::ext::subtypecheck,
                                METH_O | METH_CLASS,
                                "ImageType.check(putative)\n"
                                "\t-> Check that an instance is of this type (or a subtype)\n" },
                        {
                            "typecheck",
                                (PyCFunction)py::ext::typecheck,
                                METH_O | METH_CLASS,
                                "ImageType.typecheck(putative)\n"
                                "\t-> Check that an instance is strictly an instance of this type\n" },
                        {
                            "new",
                                (PyCFunction)py::ext::image::newfromsize<ImageType, BufferType>,
                                METH_VARARGS | METH_KEYWORDS | METH_STATIC,
                                "ImageType.new(width, height, planes=1, fill=0x00, nbits=8, is_signed=False)\n"
                                "\t-> Return a new image of size (width, height) \n"
                                "\t   optionally specifying: \n"
                                "\t - number of color channels (planes) \n"
                                "\t - a default fill value (fill) \n"
                                "\t - number of bits per value and/or the signedness (nbits, is_signed)\n" },
                        {
                            "merge",
                                (PyCFunction)py::ext::image::newfrommerge<ImageType, BufferType>,
                                METH_O | METH_CLASS,
                                "ImageType.merge(tuple(ImageType...))\n"
                                "\t-> Return a new image, of the same size as the images in the tuple, \n"
                                "\t   with planar data merged from all images \n" },
                        {
                            "frombuffer",
                                (PyCFunction)py::ext::image::newfrombuffer<ImageType, BufferType>,
                                METH_O | METH_STATIC,
                                "ImageType.frombuffer(buffer)\n"
                                "\t-> Return a new image based on an im.Buffer instance\n" },
                        {
                            "fromimage",
                                (PyCFunction)py::ext::image::newfromimage<ImageType, BufferType>,
                                METH_O | METH_STATIC,
                                "ImageType.fromimage(image)\n"
                                "\t-> Return a new image based on an existing image instance\n" },
                        {
                            "write",
                                (PyCFunction)py::ext::image::write<ImageType, BufferType>,
                                METH_VARARGS | METH_KEYWORDS,
                                "image.write(destination=\"\", file=None, as_blob=False, options={})\n"
                                "\t-> Format and write image data to file or blob\n"
                                "\t   specifying one of: \n"
                                "\t - a destination file path (destination)\n"
                                "\t - a filehandle opened for writing (file)\n"
                                "\t - a boolean flag requiring data to be returned as bytes (as_blob)\n"
                                "\t   optionally specifying: \n"
                                "\t - format-specific write options (options) \n"
                                "\t   NOTE: \n"
                                "\t - options must contain a 'format' entry, specifying the output format \n"
                                "\t   when write() is called without a destination path. \n" },
                        {
                            "split",
                                (PyCFunction)py::ext::image::split<ImageType, BufferType>,
                                METH_NOARGS,
                                "image.split()\n"
                                "\t-> Return a tuple of new images, one for each plane in the original,\n"
                                "\t   containing a monochrome copy of the given planes' data\n" },
                        {
                            "plane_at",
                                (PyCFunction)py::ext::image::plane_at<ImageType, BufferType>,
                                METH_VARARGS | METH_KEYWORDS,
                                "image.plane_at(index=0, plane=\"R|G|B|A|...\")\n"
                                "\t-> Return a new image with the planar data from the specified plane,\n"
                                "\t   which the specification may be made by either:\n"
                                "\t - a numeric index (via the `index` kwarg), or\n"
                                "\t - a lexical index (via the `plane` kwarg)\n" },
                        {
                            "add_alpha",
                                (PyCFunction)py::ext::image::add_alpha<ImageType, BufferType>,
                                METH_NOARGS,
                                "image.add_alpha()\n"
                                "\t-> Add an alpha channel (if appropriate) to a copy of the image,\n"
                                "\t   and return the copy. \n"},
                        {
                            "remove_alpha",
                                (PyCFunction)py::ext::image::remove_alpha<ImageType, BufferType>,
                                METH_NOARGS,
                                "image.remove_alpha()\n"
                                "\t-> Remove the alpha channel (if appropriate) from a copy of the image,\n"
                                "\t   and return the copy. \n"},
                        {
                            "format_read_opts",
                                (PyCFunction)py::ext::image::format_read_opts<ImageType, BufferType>,
                                METH_NOARGS,
                                "image.format_read_opts()\n"
                                "\t-> Get the read options as a formatted JSON string\n" },
                        {
                            "format_write_opts",
                                (PyCFunction)py::ext::image::format_write_opts<ImageType, BufferType>,
                                METH_NOARGS,
                                "image.format_write_opts()\n"
                                "\t-> Get the write options as a formatted JSON string\n" },
                        {
                            "dump_read_opts",
                                (PyCFunction)py::ext::image::dump_read_opts<ImageType, BufferType>,
                                METH_VARARGS | METH_KEYWORDS,
                                "image.dump_read_opts()\n"
                                "\t-> Dump the read options to a JSON file\n" },
                        {
                            "dump_write_opts",
                                (PyCFunction)py::ext::image::dump_write_opts<ImageType, BufferType>,
                                METH_VARARGS | METH_KEYWORDS,
                                "image.dump_write_opts()\n"
                                "\t-> Dump the write options to a JSON file\n" },
                        { nullptr, nullptr, 0, nullptr }
                    };
                    return basics;
                }
                
            } /* namespace methods */
            
        } /* namespace image */
        
    } /* namespace ext */
    
}

#endif /// LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_IMAGEMETHODS_HH_