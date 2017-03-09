/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)
/// Adapted from http://stackoverflow.com/a/31633962/298171

#pragma once
#include <typeinfo>
#include <utility>

#define TYPENAME(arg) typeid(arg).name()

namespace runtime {
    
    /// actual function to demangle an allegedly mangled thing
    char const* demangle(char const* const symbol) noexcept;
    
    /// convenience function template to stringify a name of a type,
    /// either per an explicit specialization:
    ///     char const* mytypename = runtime::nameof<SomeType>();
    template <typename NameType>
    char const* nameof() {
        try {
            return demangle(TYPENAME(NameType));
        } catch (std::bad_typeid const&) {
            return "<unknown>";
        }
    }
    
    ///  … or as implied by an instance argument:
    ///     char const* myinstancetypename = runtime::nameof(someinstance);
    template <typename ArgType>
    char const* nameof(ArgType&& argument) {
        try {
            return demangle(TYPENAME(std::forward<ArgType>(argument)));
        } catch (std::bad_typeid const&) {
            return "<unknown>";
        }
    }
    
} /* namespace runtime */

#undef TYPENAME