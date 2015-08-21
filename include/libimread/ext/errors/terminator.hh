/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)
/// Adapted from http://stackoverflow.com/a/31633962/298171

#ifndef LIBIMREAD_EXT_ERRORS_TERMINATOR_HH_
#define LIBIMREAD_EXT_ERRORS_TERMINATOR_HH_

#include "demangle.hh"
#include "backtrace.hh"

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <exception>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

namespace {
    
    __attribute__((noreturn))
    void backtrace_on_terminate() noexcept;
    
    // static_assert(std::is_same<std::terminate_handler,
    //                            decltype(&backtrace_on_terminate)>{},
    //                            "Type mismatch on return from backtrace_on_terminate()!");
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wglobal-constructors"
    #pragma clang diagnostic ignored "-Wexit-time-destructors"
    std::unique_ptr<std::remove_pointer_t<std::terminate_handler>,
                    decltype(std::set_terminate)&> terminate_handler {
                        std::set_terminate(backtrace_on_terminate),
                        std::set_terminate };
    #pragma clang diagnostic pop
    
    __attribute__((noreturn))
    void backtrace_on_terminate() noexcept {
        std::set_terminate(terminate_handler.release()); /// avoid infinite looping
        backtrace(std::clog);
        if (std::exception_ptr ep = std::current_exception()) {
            try {
                std::rethrow_exception(ep);
            } catch (std::exception const& e) {
                std::clog << "backtrace: unhandled exception std::exception:what(): " << e.what() << std::endl;
            } catch (...) {
                if (std::type_info * et = abi::__cxa_current_exception_type()) {
                    std::clog << "backtrace: unhandled exception type: "
                              << get_demangled_name(et->name())
                              << std::endl;
                } else {
                    std::clog << "backtrace: unhandled unknown exception" << std::endl;
                }
            }
        }
        std::_Exit(EXIT_FAILURE);
    }

}

#endif /// LIBIMREAD_EXT_ERRORS_TERMINATOR_HH_