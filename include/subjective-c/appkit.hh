/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef SUBJECTIVE_C_APPKIT_HH
#define SUBJECTIVE_C_APPKIT_HH

#import  <AppKit/AppKit.h>
#include "types.hh"
#include "traits.hh"

namespace objc {
    
    namespace appkit {
        
        template <typename OCType>
        using ocpointer_t = typename std::decay_t<std::conditional_t<
                                                  std::is_pointer<OCType>::value, OCType,
                                                               std::add_pointer_t<OCType>>>;
        
        template <typename OCType>
        using octype_t = typename std::decay_t<std::conditional_t<
                                               std::is_pointer<OCType>::value,
                                               std::remove_pointer_t<OCType>, OCType>>;
        
        template <typename OCType>
        BOOL can_paste(NSPasteboard* board = nil) noexcept {
            static_assert(objc::traits::is_object<ocpointer_t<OCType>>::value,
                          "objc::appkit::can_paste<OCType> requires an Objective-C object type");
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            return [board canReadObjectForClasses:@[ [octype_t<OCType> class] ]
                                          options:@{}];
        }
        
        template <typename OCType>
        __attribute__((ns_returns_retained))
        ocpointer_t<OCType> paste(NSPasteboard* board = nil) noexcept {
            static_assert(objc::traits::is_object<ocpointer_t<OCType>>::value,
                          "objc::appkit::paste<OCType> requires an Objective-C object type");
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            if (!can_paste<ocpointer_t<OCType>>(board)) { return nil; }
            NSArray* out = [board readObjectsForClasses:@[ [octype_t<OCType> class] ]
                                                options:@{}];
            
            /// array is nil on error -- but empty if the call to
            /// `readObjectsForClasses:options:` comes up short… SOOO:
            return out == nil ? nil : out[0];
        }
        
        template <typename ...OCTypes>
        BOOL copy_to(NSPasteboard* board, OCTypes... objects) noexcept {
            static_assert(objc::traits::detail::are_object_pointers<OCTypes...>::value,
                          "objc::appkit::copy_to<...OCTypes> requires Objective-C object types");
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            __attribute__((__unused__))
            NSInteger changecount = [board clearContents];
            return [board writeObjects:@[ objects... ]];
        }
        
        template <typename ...OCTypes> inline
        BOOL copy(OCTypes... objects) noexcept {
            static_assert(objc::traits::detail::are_object_pointers<OCTypes...>::value,
                          "objc::appkit::copy<...OCTypes> requires Objective-C object types");
            return copy_to<OCTypes...>([NSPasteboard generalPasteboard],
                                        objects...);
        }
        
    }
    
}

#endif /// SUBJECTIVE_C_APPKIT_HH