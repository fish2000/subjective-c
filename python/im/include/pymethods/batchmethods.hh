
#ifndef LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_BATCHMETHODS_HH_
#define LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_BATCHMETHODS_HH_

#include <memory>
#include <string>
#include <Python.h>
#include <structmember.h>

#include "../check.hh"
#include "../detail.hpp"

#include "typecheck.hh"
#include "../models/batchmodel.hh"

namespace py {
    
    namespace ext {
        
        using im::byte;
        using im::options_map;
        
        namespace batch {
            
            PyObject* createnew(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
                return py::convert(new BatchModel());
            }
            
            int init(PyObject* self, PyObject* args, PyObject* kwargs) {
                // BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return 0;
            }
            
            /// __repr__ implementation
            PyObject* repr(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__repr__();
            }
            
            /// __str__ implementaton -- same as __repr__
            PyObject* str(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__repr__();
            }
            
            /// __hash__ implementation
            long hash(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__hash__();
            }
            
            /// cmp(batch0, batch1) implementaton
            int compare(PyObject* pylhs, PyObject* pyrhs) {
                BatchModel* batch0 = reinterpret_cast<BatchModel*>(pylhs);
                BatchModel* batch1 = reinterpret_cast<BatchModel*>(pyrhs);
                PyObject* lhs_compare = py::convert(batch0->__len__());
                PyObject* rhs_compare = py::convert(batch1->__len__());
                int out = PyObject_Compare(lhs_compare, rhs_compare);
                Py_DECREF(lhs_compare);
                Py_DECREF(rhs_compare);
                return out;
            }
            
            /// __len__ implementaton
            Py_ssize_t length(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__len__();
            }
            
            /// sq_concat
            PyObject* concat(PyObject* lhs, PyObject* rhs) {
                return py::convert(new BatchModel(lhs, rhs));
            }
            
            /// sq_repeat
            PyObject* repeat(PyObject* basis, Py_ssize_t repeat) {
                return py::convert(new BatchModel(basis, repeat));
            }
            
            /// sq_item
            PyObject* atindex(PyObject* self, Py_ssize_t idx) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__index__(idx);
            }
            
            /// sq_ass_item
            int valueatindex(PyObject* self, Py_ssize_t idx, PyObject* value) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return batch->__index__(idx, value) == nullptr ? -1 : 1;
            }
            
            /// sq_inplace_concat
            PyObject* inplace_concat(PyObject* lhs, PyObject* rhs) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(lhs);
                BatchModel* additional = reinterpret_cast<BatchModel*>(rhs);
                PyObject* list = additional->as_pylist();
                bool did_extend = batch->extend(list);
                return did_extend ? py::convert(batch) : nullptr; /// propagate error
            }
            
            /// DEALLOCATE
            void dealloc(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                delete batch;
            }
            
            /// CLEAR
            int clear(PyObject* self) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                batch->cleanup(true);
                return 0;
            }
            
            /// TRAVERSE
            int traverse(PyObject* self, visitproc visit, void* arg) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                batch->vacay(visit, arg);
                return 0;
            }
            
            PyObject* append(PyObject* self, PyObject* obj) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                bool did_append = batch->append(obj);
                return did_append ? py::convert(batch) : nullptr;
            }
            
            PyObject* count(PyObject* self, PyObject* obj) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                return py::convert(batch->count(obj));
            }
            
            PyObject* extend(PyObject* self, PyObject* obj) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                bool did_extend = batch->extend(obj);
                return did_extend ? py::convert(batch) : nullptr;
            }
            
            PyObject* indexof(PyObject* self, PyObject* args, PyObject* kwargs) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                PyObject* value = nullptr;
                int start = 0;
                int stop = -1;
                char const* keywords[] = { "value", "start", "stop", nullptr };
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "O|ii:index", const_cast<char**>(keywords),
                    &value,             /// "value", PyObject*, thing to look for
                    &start,             /// "start", int, begin() iterator index
                    &stop))             /// "stop", int, end() iterator index
                {
                    return nullptr;
                }
                
                /// will throw ValueErrror if need be:
                return py::convert(batch->index(value, start, stop));
            }
            
            PyObject* insert(PyObject* self, PyObject* args, PyObject* kwargs) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                PyObject* obj = nullptr;
                int idx = 0;
                char const* keywords[] = { "index", "object", nullptr };
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "iO:insert", const_cast<char**>(keywords),
                    &idx,        /// "index", int, where to insert
                    &obj))       /// "object", PyObject*, what to insert
                {
                    return nullptr;
                }
                
                /// will throw ValueErrror if need be:
                return py::convert(batch->insert(idx, obj));
            }
            
            PyObject* pop(PyObject* self, PyObject* args, PyObject* kwargs) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                int idx = -1;
                char const* keywords[] = { "index", nullptr };
                
                if (!PyArg_ParseTupleAndKeywords(
                    args, kwargs, "|i:pop", const_cast<char**>(keywords),
                    &idx))       /// "index", int, from whence to pop()
                {
                    return nullptr;
                }
                
                /// will throw ValueErrror if need be:
                return py::convert(batch->pop(idx));
            }
            
            PyObject* removeobj(PyObject* self, PyObject* obj) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                bool did_remove = batch->remove(obj);
                return did_remove ? py::convert(batch) : nullptr;
            }
            
            PyObject* reverse(PyObject* self, PyObject*) {
                BatchModel* batch = reinterpret_cast<BatchModel*>(self);
                batch->reverse();
                return py::None();
            }
            
            namespace methods {
                
                PySequenceMethods* sequence() {
                    static PySequenceMethods sequencemethods = {
                        (lenfunc)py::ext::batch::length,
                        (binaryfunc)py::ext::batch::concat,
                        (ssizeargfunc)py::ext::batch::repeat,
                        (ssizeargfunc)py::ext::batch::atindex,
                        0, /*(ssizeobjargproc)py::ext::batch::valueatindex,*/
                        0, 
                        0, /*(binaryfunc)py::ext::batch::inplace_concat,*/
                        0
                    };
                    return &sequencemethods;
                }
                
                // PyGetSetDef* getset() {
                //     static PyGetSetDef getsets[] = {
                //         {
                //             (char*)"__array_interface__",
                //                 (getter)py::ext::buffer::get_array_interface<BufferType, PythonBufferType>,
                //                 nullptr,
                //                 (char*)"NumPy array interface (Python API) -> dict\n",
                //                 nullptr },
                //         {
                //             (char*)"__array_struct__",
                //                 (getter)py::ext::buffer::get_array_struct<BufferType, PythonBufferType>,
                //                 nullptr,
                //                 (char*)"NumPy array interface (C-level API) -> PyCObject\n",
                //                 nullptr },
                //         { nullptr, nullptr, nullptr, nullptr, nullptr }
                //     };
                //     return getsets;
                // }
                
                PyMethodDef* basic() {
                    static PyMethodDef basics[] = {
                        {
                            "check",
                                (PyCFunction)py::ext::subtypecheck,
                                METH_O | METH_CLASS,
                                "Batch.check(putative)\n"
                                "\t-> Check that an instance is of this type (or a subtype)\n" },
                        {
                            "typecheck",
                                (PyCFunction)py::ext::typecheck,
                                METH_O | METH_CLASS,
                                "Batch.typecheck(putative)\n"
                                "\t-> Check that an instance is strictly an instance of this type\n" },
                        {
                            "append",
                                (PyCFunction)py::ext::batch::append,
                                METH_O,
                                "batch.append(object)\n"
                                "\t-> Append buffer-compatible object to end\n" },
                        {
                            "count",
                                (PyCFunction)py::ext::batch::count,
                                METH_O,
                                "batch.count(value)\n"
                                "\t-> Return number of occurences of value\n" },
                        {
                            "extend",
                                (PyCFunction)py::ext::batch::extend,
                                METH_O,
                                "batch.extend(iterable)\n"
                                "\t-> Extend by appending buffer-compatible elements from the iterable\n" },
                        {
                            "index",
                                (PyCFunction)py::ext::batch::indexof,
                                METH_VARARGS | METH_KEYWORDS,
                                "batch.index(value, [start, [stop]])\n"
                                "\t-> Return first index of value.\n"
                                "\t   Raises ValueError if the value is not present.\n" },
                        {
                            "insert",
                                (PyCFunction)py::ext::batch::insert,
                                METH_VARARGS | METH_KEYWORDS,
                                "batch.insert(index, object)\n"
                                "\t-> Insert object before index\n" },
                        {
                            "pop",
                                (PyCFunction)py::ext::batch::pop,
                                METH_VARARGS | METH_KEYWORDS,
                                "batch.pop([index])\n"
                                "\t-> Remove and return item at index (default last).\n"
                                "\t   Raises IndexError if list is empty or index is out of range.\n" },
                        {
                            "remove",
                                (PyCFunction)py::ext::batch::removeobj,
                                METH_O,
                                "batch.remove(value)\n"
                                "\t-> Remove first occurrence of value.\n"
                                "\t   Raises ValueError if the value is not present.\n" },
                        {
                            "reverse",
                                (PyCFunction)py::ext::batch::reverse,
                                METH_NOARGS,
                                "batch.reverse()\n"
                                "\t-> Reverse the batch ***IN PLACE*** \n" },
                        { nullptr, nullptr, 0, nullptr }
                    };
                    return basics;
                }
                
            }; /* namespace methods */
            
        }; /* namespace batch */
    
    }; /* namespace ext */

}; /* namespace py */


#endif /// LIBIMREAD_PYTHON_IM_INCLUDE_PYMETHODS_BATCHMETHODS_HH_