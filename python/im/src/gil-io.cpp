
#include "gil-io.hpp"
#include <libimread/errors.hh>

namespace py {
    
    namespace gil {
        
        with::with(PyObject* fileobject)
            :state(nullptr), source(reinterpret_cast<PyFileObject*>(fileobject))
            ,file(nullptr),  active(PyFile_Check(fileobject))
            {
                if (active) { init(); }
            }
        
        with::with(PyFileObject* fileobject)
            :state(nullptr), source(fileobject)
            ,file(nullptr),  active(PyFile_Check(fileobject))
            {
                if (active) { init(); }
            }
        
        with::with(std::nullptr_t no)
            :state(nullptr), source(nullptr)
            ,file(nullptr),  active(false)
            {}
        
        with::~with() {
            if (active) { restore(); }
        }
        
        void with::init() {
            Py_INCREF(source);
            file = PyFile_AsFile(reinterpret_cast<PyObject*>(source));
            PyFile_IncUseCount(source);
            state = PyEval_SaveThread();
        }
        
        void with::restore() {
            PyEval_RestoreThread(state);
            PyFile_DecUseCount(source);
            Py_DECREF(source);
            file = nullptr;
            active = false;
        }
        
        std::unique_ptr<im::handle::source> source() const {
            if (!active) {
                imread_raise(CannotReadError,
                    "py::gil::with::source():",
                    "\tGIL guard not active");
            }
            return std::make_unique<im::handle::source>(file);
        }
        
        std::unique_ptr<im::handle::sink> sink() const {
            if (!active) {
                imread_raise(CannotWriteError,
                    "py::gil::with::sink():",
                    "\tGIL guard not active");
            }
            return std::make_unique<im::handle::sink>(file);
        }
        
        
    } /* namespace gil */
    
} /* namespace py */
