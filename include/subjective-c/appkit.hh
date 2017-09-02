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
        bool can_paste(NSPasteboard* board = nil) noexcept
        {
            static_assert(objc::traits::is_object<
                          objc::ocpointer_t<OCType>>::value,
                         "objc::appkit::can_paste<OCType> requires an Objective-C object type");
            
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            return objc::to_bool([board canReadObjectForClasses:@[ [objc::octype_t<OCType> class] ]
                                                        options:@{ }]);
        }
        
        template <typename OCType>
        __attribute__((ns_returns_retained))
        objc::ocpointer_t<OCType> paste(NSPasteboard* board = nil) noexcept
        {
            static_assert(objc::traits::is_object<
                          objc::ocpointer_t<OCType>>::value,
                         "objc::appkit::paste<OCType> requires an Objective-C object type");
            
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            if (!objc::appkit::can_paste<OCType>(board)) { return nil; }
            
            NSArray* out = [board readObjectsForClasses:@[ [objc::octype_t<OCType> class] ]
                                                options:@{ }];
            
            /// array is nil on error -- but empty if the call to
            /// `readObjectsForClasses:options:` comes up short… SOOO:
            return out == nil ? nil : out[0];
        }
        
        template <typename ...OCTypes>
        bool copy_to(NSPasteboard* board, OCTypes... objects) noexcept
        {
            static_assert(sizeof...(OCTypes) > 0,
                         "objc::appkit::copy_to<...OCTypes> requires at least one id<NSPasteboardWriting> object");
            static_assert(objc::traits::detail::are_object_pointers<
                          objc::ocpointer_t<OCTypes>...>::value,
                         "objc::appkit::copy_to<...OCTypes> requires Objective-C objects");
            
            if (!board) { board = [NSPasteboard generalPasteboard]; }
            NSArray< NSObject<NSPasteboardWriting>* >* copyables = @[ objects... ];
            
            __attribute__((__unused__))
            NSInteger changecount = [board clearContents];
            return objc::to_bool([board writeObjects:copyables]);
        }
        
        template <typename ...OCTypes>
        bool copy(OCTypes... objects) noexcept
        {
            static_assert(sizeof...(OCTypes) > 0,
                         "objc::appkit::copy<...OCTypes> requires at least one id<NSPasteboardWriting> object");
            static_assert(objc::traits::detail::are_object_pointers<
                          objc::ocpointer_t<OCTypes>...>::value,
                         "objc::appkit::copy<...OCTypes> requires Objective-C objects");
            
            /// objc::appkit::copy_to<…>() defaults to using the general pasteboard:
            return objc::appkit::copy_to<OCTypes...>(nil, objects...);
        }
        
    }
    
}

#endif /// SUBJECTIVE_C_APPKIT_HH