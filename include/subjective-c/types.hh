/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef SUBJECTIVE_C_TYPES_HH
#define SUBJECTIVE_C_TYPES_HH

#include <cstdlib>
#include <utility>
#include <type_traits>
#import  <Foundation/Foundation.h>
#import  <objc/message.h>
#import  <objc/runtime.h>

namespace objc {
    
    /// pointer swap
    template <typename T> inline
    void swap(T*& oA, T*& oB) noexcept {
        T* oT = oA; oA = oB; oB = oT;
    }
    
    /// bridge cast
    template <typename T, typename U>
    __attribute__((__always_inline__))
    T bridge(U castee) {
        #if __has_feature(objc_arc)
            return (__bridge T)castee;
        #else
            return (T)castee;
        #endif
    }
    
    /// bridge-retain cast --
    /// kind of a NOOp in MRC-mode
    /// (which currently we don't do ARC anyway)
    template <typename T, typename U>
    __attribute__((__always_inline__))
    T bridgeretain(U castee) {
        #if __has_feature(objc_arc)
            return (__bridge_retained T)castee;
        #else
            return (T)castee;
        #endif
    }
    
    /// bridge-transfer cast --
    /// kind of a NOOp in MRC-mode
    /// (which currently we don't do ARC anyway)
    template <typename T, typename U>
    __attribute__((__always_inline__))
    T bridgetransfer(U castee) {
        #if __has_feature(objc_arc)
            return (__bridge_transfer T)castee;
        #else
            return (T)castee;
        #endif
    }
    
    /// block type alias --
    /// because `objc::block<T> thing`
    /// looks better than `__block T thing` IN MY HUMBLE OPINION
    
    template <typename Type>
    using block_t = Type
        __attribute__((__blocks__(byref)));
    
    template <typename Type>
    using block = __block block_t<typename std::remove_cv<Type>::type>
        __attribute__((__always_inline__));
    
    /// namespaced references,
    /// for everything we use from the objective-c type system
    
    namespace types {
        
        using ID = ::id __attribute__((NSObject));
        using NS = NSObject __attribute__((NSObject));
        using object_t = struct ::objc_object;
        using super_t = struct ::objc_super;
        using selector = ::SEL;
        using cls = ::Class;
        using method = ::Method;
        using implement = ::IMP;
        using boolean = ::BOOL;
        
        using rID = std::add_rvalue_reference_t<ID>;
        using xID = std::add_lvalue_reference_t<ID>;
        using tID = std::remove_reference_t<rID>;
        
        using rSEL = std::add_rvalue_reference_t<selector>;
        using xSEL = std::add_lvalue_reference_t<selector>;
        using tSEL = std::remove_reference_t<rSEL>;
        
    }
    
    /// function-pointer templates for wrapping objc_msgSend() with a bunch of possible sigs
    
    template <typename Return, typename ...Args>
    using return_sender_t = std::add_pointer_t<Return(types::ID, types::selector, Args...)>;
    
    template <typename ...Args>
    using void_sender_t = std::add_pointer_t<void(types::ID, types::selector, Args...)>;
    
    template <typename ...Args>
    using object_sender_t = std::add_pointer_t<types::ID(types::ID, types::selector, Args...)>;
    
    /// Type aliases for forcing either the pointer or the non-pointer typename of
    /// an Objective-C type. For example:
    ///
    /// using T = NSString;             // typename without pointer
    /// using octype = octype_t<T>;     // octype is “NSString”
    /// using ocptr = ocpointer_t<T>;   // ocptr is “NSString*”
    ///
    /// using U = NSData*;              // typename plus pointer
    /// using octype = octype_t<U>;     // octype is “NSData”
    /// using ocptr = ocpointer_t<U>;   // ocptr is “NSData*”
    
    template <typename OCType>
    using ocpointer_t = typename std::decay_t<std::conditional_t<
                                              std::is_pointer<OCType>::value, OCType,
                                                           std::add_pointer_t<OCType>>>;
    
    template <typename OCType>
    using octype_t = typename std::decay_t<std::conditional_t<
                                           std::is_pointer<OCType>::value,
                                           std::remove_pointer_t<OCType>, OCType>>;
    
    /// Helpers for Objective-C's highly particular boolean-value
    /// macro typedef'd whatever the fuck type stuff
    /// objc::boolean(bool_value) -> YES or NO
    /// objc::to_bool(BOOL_value) -> true or false
    
    __attribute__((__always_inline__)) types::boolean boolean(bool value);
    __attribute__((__always_inline__)) bool to_bool(types::boolean value);
    
} /* namespace objc */


#endif /// SUBJECTIVE_C_TYPES_HH
