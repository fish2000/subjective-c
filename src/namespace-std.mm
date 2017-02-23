/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <subjective-c/namespace-std.hh>

namespace std {
    
    template <>
    void swap(objc::selector& s0, objc::selector& s1) {
        s0.swap(s1);
    }
    
    template <>
    void swap(objc::id& s0, objc::id& s1)
         noexcept(is_nothrow_move_constructible<objc::id>::value &&
                  is_nothrow_move_assignable<objc::id>::value) {
        s0.swap(s1);
    }
    
    template <>
    void swap(objc::types::cls& cls0, objc::types::cls& cls1)
        noexcept(is_nothrow_move_constructible<objc::types::cls>::value
              && is_nothrow_move_assignable<objc::types::cls>::value) {
        std::swap(cls0, cls1);
    }
    
}