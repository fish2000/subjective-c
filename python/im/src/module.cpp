
#include <cstddef>
#include "module.hpp"

PyTypeObject HybridImageModel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                                                  /* ob_size */
    "im.HybridImage",                                                   /* tp_name */
    sizeof(HybridImageModel),                                           /* tp_basicsize */
    0,                                                                  /* tp_itemsize */
    (destructor)py::image::dealloc<HalideNumpyImage>,                   /* tp_dealloc */
    0,                                                                  /* tp_print */
    0,                                                                  /* tp_getattr */
    0,                                                                  /* tp_setattr */
    0,                                                                  /* tp_compare */
    (reprfunc)py::image::repr<HalideNumpyImage>,                        /* tp_repr */
    0,                                                                  /* tp_as_number */
    &HybridImage_SequenceMethods,                                       /* tp_as_sequence */
    0,                                                                  /* tp_as_mapping */
    (hashfunc)py::image::hash<HalideNumpyImage>,                        /* tp_hash */
    0,                                                                  /* tp_call */
    (reprfunc)py::image::str<HalideNumpyImage>,                         /* tp_str */
    (getattrofunc)PyObject_GenericGetAttr,                              /* tp_getattro */
    (setattrofunc)PyObject_GenericSetAttr,                              /* tp_setattro */
    &HybridImage_Buffer3000Methods,                                     /* tp_as_buffer */
    HybridImageModel_TypeFlags,                                         /* tp_flags */
    "Python bindings for NumPy Halide bridge",                          /* tp_doc */
    0,                                                                  /* tp_traverse */
    0,                                                                  /* tp_clear */
    0,                                                                  /* tp_richcompare */
    0,                                                                  /* tp_weaklistoffset */
    0,                                                                  /* tp_iter */
    0,                                                                  /* tp_iternext */
    HybridImage_methods,                                                /* tp_methods */
    0,                                                                  /* tp_members */
    HybridImage_getset,                                                 /* tp_getset */
    0,                                                                  /* tp_base */
    0,                                                                  /* tp_dict */
    0,                                                                  /* tp_descr_get */
    0,                                                                  /* tp_descr_set */
    0,                                                                  /* tp_dictoffset */
    (initproc)py::image::init<HalideNumpyImage, HybridFactory>,         /* tp_init */
    0,                                                                  /* tp_alloc */
    py::image::createnew<HalideNumpyImage>,                             /* tp_new */
    0,                                                                  /* tp_free */
    0,                                                                  /* tp_is_gc */
    0,                                                                  /* tp_bases */
    0,                                                                  /* tp_mro */
    0,                                                                  /* tp_cache */
    0,                                                                  /* tp_subclasses */
    0,                                                                  /* tp_weaklist */
    0,                                                                  /* tp_del */
    0,                                                                  /* tp_version_tag */
};

PyTypeObject BufferModel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                                                  /* ob_size */
    "im.Buffer",                                                        /* tp_name */
    sizeof(BufferModel),                                                /* tp_basicsize */
    0,                                                                  /* tp_itemsize */
    (destructor)py::ext::buffer::dealloc<buffer_t>,                     /* tp_dealloc */
    0,                                                                  /* tp_print */
    0,                                                                  /* tp_getattr */
    0,                                                                  /* tp_setattr */
    0,                                                                  /* tp_compare */
    (reprfunc)py::ext::buffer::repr<buffer_t>,                          /* tp_repr */
    0,                                                                  /* tp_as_number */
    &Buffer_SequenceMethods,                                            /* tp_as_sequence */
    0,                                                                  /* tp_as_mapping */
    0,                                                                  /* tp_hash */
    0,                                                                  /* tp_call */
    (reprfunc)py::ext::buffer::str<buffer_t>,                           /* tp_str */
    (getattrofunc)PyObject_GenericGetAttr,                              /* tp_getattro */
    (setattrofunc)PyObject_GenericSetAttr,                              /* tp_setattro */
    &Buffer_Buffer3000Methods,                                          /* tp_as_buffer */
    py::ext::BufferModel::typeflags(),                                  /* tp_flags */
    py::ext::BufferModel::typedoc(),                                    /* tp_doc */
    (traverseproc)py::ext::buffer::traverse<buffer_t>,                  /* tp_traverse */
    (inquiry)py::ext::buffer::clear<buffer_t>,                          /* tp_clear */
    0,                                                                  /* tp_richcompare */
    py::detail::offset(&BufferModel::weakrefs),                         /* tp_weaklistoffset */
    0,                                                                  /* tp_iter */
    0,                                                                  /* tp_iternext */
    Buffer_methods,                                                     /* tp_methods */
    0,                                                                  /* tp_members */
    Buffer_getset,                                                      /* tp_getset */
    0,                                                                  /* tp_base */
    0,                                                                  /* tp_dict */
    0,                                                                  /* tp_descr_get */
    0,                                                                  /* tp_descr_set */
    0,                                                                  /* tp_dictoffset */
    (initproc)py::ext::buffer::init<buffer_t>,                          /* tp_init */
    0,                                                                  /* tp_alloc */
    py::ext::buffer::createnew<buffer_t>,                               /* tp_new */
    0,                                                                  /* tp_free */
    0,                                                                  /* tp_is_gc */
    0,                                                                  /* tp_bases */
    0,                                                                  /* tp_mro */
    0,                                                                  /* tp_cache */
    0,                                                                  /* tp_subclasses */
    0,                                                                  /* tp_weaklist */
    0,                                                                  /* tp_del */
    0,                                                                  /* tp_version_tag */
};

PyTypeObject ImageModel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                                                  /* ob_size */
    "im.Image",                                                         /* tp_name */
    sizeof(ImageModel),                                                 /* tp_basicsize */
    0,                                                                  /* tp_itemsize */
    (destructor)py::ext::image::dealloc<HalideNumpyImage, buffer_t>,    /* tp_dealloc */
    0,                                                                  /* tp_print */
    0,                                                                  /* tp_getattr */
    0,                                                                  /* tp_setattr */
    0,                                                                  /* tp_compare */
    (reprfunc)py::ext::image::repr<HalideNumpyImage, buffer_t>,         /* tp_repr */
    0,                                                                  /* tp_as_number */
    &Image_SequenceMethods,                                             /* tp_as_sequence */
    0,                                                                  /* tp_as_mapping */
    (hashfunc)py::ext::image::hash<HalideNumpyImage, buffer_t>,         /* tp_hash */
    0,                                                                  /* tp_call */
    (reprfunc)py::ext::image::str<HalideNumpyImage, buffer_t>,          /* tp_str */
    (getattrofunc)PyObject_GenericGetAttr,                              /* tp_getattro */
    (setattrofunc)PyObject_GenericSetAttr,                              /* tp_setattro */
    &Image_Buffer3000Methods,                                           /* tp_as_buffer */
    py::ext::ImageModel::typeflags(),                                   /* tp_flags */
    py::ext::ImageModel::typedoc(),                                     /* tp_doc */
    (traverseproc)py::ext::image::traverse<HalideNumpyImage, buffer_t>, /* tp_traverse */
    (inquiry)py::ext::image::clear<HalideNumpyImage, buffer_t>,         /* tp_clear */
    0,                                                                  /* tp_richcompare */
    py::detail::offset(&ImageModel::weakrefs),                          /* tp_weaklistoffset */
    0,                                                                  /* tp_iter */
    0,                                                                  /* tp_iternext */
    Image_methods,                                                      /* tp_methods */
    0,                                                                  /* tp_members */
    Image_getset,                                                       /* tp_getset */
    0,                                                                  /* tp_base */
    0,                                                                  /* tp_dict */
    0,                                                                  /* tp_descr_get */
    0,                                                                  /* tp_descr_set */
    0,                                                                  /* tp_dictoffset */
    (initproc)py::ext::image::init<HalideNumpyImage, buffer_t>,         /* tp_init */
    0,                                                                  /* tp_alloc */
    py::ext::image::createnew<HalideNumpyImage, buffer_t>,              /* tp_new */
    0,                                                                  /* tp_free */
    0,                                                                  /* tp_is_gc */
    0,                                                                  /* tp_bases */
    0,                                                                  /* tp_mro */
    0,                                                                  /* tp_cache */
    0,                                                                  /* tp_subclasses */
    0,                                                                  /* tp_weaklist */
    0,                                                                  /* tp_del */
    0,                                                                  /* tp_version_tag */
};

PyTypeObject ImageBufferModel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                                                      /* ob_size */
    "im.Image.Buffer",                                                      /* tp_name */
    sizeof(ImageBufferModel),                                               /* tp_basicsize */
    0,                                                                      /* tp_itemsize */
    (destructor)py::ext::buffer::dealloc<buffer_t, ImageBufferModel>,       /* tp_dealloc */
    0,                                                                      /* tp_print */
    0,                                                                      /* tp_getattr */
    0,                                                                      /* tp_setattr */
    0,                                                                      /* tp_compare */
    (reprfunc)py::ext::buffer::repr<buffer_t, ImageBufferModel>,            /* tp_repr */
    0,                                                                      /* tp_as_number */
    &ImageBuffer_SequenceMethods,                                           /* tp_as_sequence */
    0,                                                                      /* tp_as_mapping */
    0,                                                                      /* tp_hash */
    0,                                                                      /* tp_call */
    (reprfunc)py::ext::buffer::str<buffer_t, ImageBufferModel>,             /* tp_str */
    (getattrofunc)PyObject_GenericGetAttr,                                  /* tp_getattro */
    (setattrofunc)PyObject_GenericSetAttr,                                  /* tp_setattro */
    &ImageBuffer_Buffer3000Methods,                                         /* tp_as_buffer */
    py::ext::ImageModel::ImageBufferModel::typeflags(),                     /* tp_flags */
    py::ext::ImageModel::ImageBufferModel::typedoc(),                       /* tp_doc */
    (traverseproc)py::ext::buffer::traverse<buffer_t, ImageBufferModel>,    /* tp_traverse */
    (inquiry)py::ext::buffer::clear<buffer_t, ImageBufferModel>,            /* tp_clear */
    0,                                                                      /* tp_richcompare */
    py::detail::offset(&ImageBufferModel::weakrefs),                        /* tp_weaklistoffset */
    0,                                                                      /* tp_iter */
    0,                                                                      /* tp_iternext */
    ImageBuffer_methods,                                                    /* tp_methods */
    0,                                                                      /* tp_members */
    ImageBuffer_getset,                                                     /* tp_getset */
    0,                                                                      /* tp_base */
    0,                                                                      /* tp_dict */
    0,                                                                      /* tp_descr_get */
    0,                                                                      /* tp_descr_set */
    0,                                                                      /* tp_dictoffset */
    (initproc)py::ext::buffer::init<buffer_t, ImageBufferModel>,            /* tp_init */
    0,                                                                      /* tp_alloc */
    py::ext::buffer::createnew<buffer_t, ImageBufferModel>,                 /* tp_new */
    0,                                                                      /* tp_free */
    0,                                                                      /* tp_is_gc */
    0,                                                                      /* tp_bases */
    0,                                                                      /* tp_mro */
    0,                                                                      /* tp_cache */
    0,                                                                      /* tp_subclasses */
    0,                                                                      /* tp_weaklist */
    0,                                                                      /* tp_del */
    0,                                                                      /* tp_version_tag */
};

static PyMethodDef module_functions[] = {
    {
        "structcode_parse",
            (PyCFunction)py::functions::structcode_parse,
            METH_VARARGS,
            "Parse struct code into list of dtype-string tuples" },
    {
        "hybridimage_check",
            (PyCFunction)py::functions::hybridimage_check,
            METH_VARARGS,
            "Boolean function to test for HybridImage instances" },
    {
        "image_check",
            (PyCFunction)py::functions::image_check,
            METH_VARARGS,
            "Boolean function to test for im.Image instances" },
    {
        "buffer_check",
            (PyCFunction)py::functions::buffer_check,
            METH_VARARGS,
            "Boolean function to test for im.Buffer instances" },
    {
        "imagebuffer_check",
            (PyCFunction)py::functions::imagebuffer_check,
            METH_VARARGS,
            "Boolean function to test for im.Image.ImageBuffer instances" },
    { nullptr, nullptr, 0, nullptr }
};

#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initim(void) {
    /// Declare some object pointers:
    /// ... one, to the new module object;
    PyObject* module;
    
    /// ... another to the tuple of image-format suffix strings …
    PyObject* format_tuple;
    
    /// Initialize Python threads and GIL state
    PyEval_InitThreads();
    
    /// Bring in NumPy's C-API
    import_array();
    
    /// Manually amend our declared types, as needed:
    /// -- Specify that im.ImageBuffer subclasses im.Buffer
    ImageBufferModel_Type.tp_base = &BufferModel_Type;
    
    /// -- Prepare im.Image.__dict__ for our customizations
    ImageModel_Type.tp_dict = PyDict_New();
    if (!ImageModel_Type.tp_dict)                 { return; }
    
    /// Check readiness of our extension type declarations (?)
    if (PyType_Ready(&HybridImageModel_Type) < 0) { return; }
    if (PyType_Ready(&BufferModel_Type) < 0)      { return; }
    if (PyType_Ready(&ImageBufferModel_Type) < 0) { return; }
    if (PyType_Ready(&ImageModel_Type) < 0)       { return; }
    
    /// Actually initialize the module object,
    /// setting up the module's external C-function table
    module = Py_InitModule3(
        "im.im", module_functions,
        "libimread python bindings");
    if (module == NULL)                           { return; }
    
    /// Add the HybridImageModel type object to the module
    Py_INCREF(&HybridImageModel_Type);
    PyModule_AddObject(module,
        "HybridImage",
        (PyObject*)&HybridImageModel_Type);
    
    /// Add the BufferModel type object to the module
    Py_INCREF(&BufferModel_Type);
    PyModule_AddObject(module,
        "Buffer",
        (PyObject*)&BufferModel_Type);
    
    /// Add the ImageBufferModel type object directly to im.Image.__dict__,
    /// such that ImageBuffer presents as an inner class of im.Image, e.g.
    /// 
    ///     im.Image.ImageBuffer
    /// 
    /// ... thanks SO! http://stackoverflow.com/q/35954016/298171
    Py_INCREF(&ImageBufferModel_Type);
    PyDict_SetItemString(ImageModel_Type.tp_dict,
        "Buffer",
        (PyObject*)&ImageBufferModel_Type);
    
    /// Add the ImageModel type object to the module
    Py_INCREF(&ImageModel_Type);
    PyModule_AddObject(module,
        "Image",
        (PyObject*)&ImageModel_Type);
    
    /// Get the master list of image format suffixes,
    /// newly formatted as a Python tuple of strings,
    /// and add this to the module as a static-ish constant
    format_tuple = py::detail::formats_as_pytuple();
    if (format_tuple == NULL)                     { return; }
    Py_INCREF(format_tuple);
    PyModule_AddObject(module,
        "formats",
        format_tuple);
    
}
