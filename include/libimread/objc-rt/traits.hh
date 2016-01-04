/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_OBJC_RT_TRAITS_HH
#define LIBIMREAD_OBJC_RT_TRAITS_HH

#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <string>
#include <tuple>
#include <array>
#include <utility>
#include <functional>
#include <type_traits>

#ifdef __APPLE__
// #import <libimread/ext/categories/NSString+STL.hh>
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <objc/message.h>
#import <objc/runtime.h>
#endif /// __APPLE__

namespace objc {
    
    namespace traits {
    
        namespace detail {
            
            /// convenience type struct that should already freaking exist
            /// to go along with std::is_convertible<From, To>
            
            template <typename From, typename To>
            using is_convertible_t = std::conditional_t<std::is_convertible<From, To>::value,
                                                        std::true_type, std::false_type>;
            
            /// detail test defs for overwrought argument-list check (see below)
            
            template <typename T, typename ...Args>
            static auto test_is_argument_list(int) -> typename T::is_argument_list_t;
            template <typename, typename ...Args>
            static auto test_is_argument_list(long) -> std::false_type;
            
            /// OLDSKOOL SFINAE 4 LYFE. Recipe courtesy WikiBooks:
            /// https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
            /// ... to test for an Objective-C object, has_isa<PointerType> checks the struct
            /// at which it points for an `isa` member (see http://stackoverflow.com/q/1990695/298171)
            /// Now this is a budget way to SFINAE I know but that is cuz my SFINAE is
            /// STRAIGHT OUTTA COMPTON DOGG this one is FOR ALL MY ISAZ
            /// 
            /// (ahem)
            ///
            /// Also the has_isa class is itself enable_if'd only for classes because
            /// specializing it for fundamental types makes it all FREAK THE GEEK OUT
            
            template <typename U, U> struct check;
            typedef char one[1];
            typedef char two[2];
            
            template <typename Target,
                      typename T = std::remove_pointer_t<
                                   std::decay_t<Target>>>
            class has_isa {
                struct detect { int isa; };
                struct composite : T, detect {};
                template <typename U> static detail::one &test(
                                      detail::check<
                                          int detect::*,
                                          &U::isa>*);
                template <typename U> static detail::two &test(...);
                public:
                    typedef T type;
                    typedef Target pointer_type;
                    enum { value = sizeof(test<composite>(0)) == 2 };
            };
            
            template <typename Target,
                      typename T = std::remove_pointer_t<
                                   std::decay_t<Target>>>
            class has_superclass {
                struct detect { int superclass; };
                struct detect_ { int super_class; };
                struct composite : T, detect, detect_ {};
                template <typename U> static detail::one &test(
                                      detail::check<
                                          int detect::*,
                                          &U::superclass>*);
                template <typename U> static detail::one &test(
                                      detail::check<
                                          int detect_::*,
                                          &U::super_class>*);
                template <typename U> static detail::two &test(...);
                public:
                    typedef T type;
                    typedef Target pointer_type;
                    enum { value = sizeof(test<composite>(0)) == 2 };
            };
            
            /// All of this following hoohah is a SFINAE-compatible reimplementation
            /// of std::common_type<T>, taken right from this document:
            ///     http://open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3843.pdf
            template <typename T, typename U>
            using ct2 = decltype(std::declval<bool>() ? std::declval<T>() : std::declval<U>());
            
            template <typename T>
            using void_t = std::conditional_t<true, void, T>;
            
            template <class, class...>
            struct ct {};
            template <class T>
            struct ct<void, T> { using type = std::decay_t<T>; };
            template <class T, class U, class ...V>
            struct ct<void_t<ct2<T, U>>, T, U, V...> : ct<void, ct2<T, U>, V...> {};
            
            template <typename ...Types>
            struct common_type : ct<void, Types...> {};
            
            template <class, class = void>
            struct has_type_member : std::false_type {};
            template <class T>
            struct has_type_member<T, void_t<typename T::type>> : std::true_type {};
            
            template <typename ...Types>
            using common_type_t = typename common_type<Types...>::type;
            template <typename ...Types>
            using has_common_type = has_type_member<common_type<Types...>>;
            
            /// objc::traits::detail::is_object_pointer<T> checks using
            /// objc::traits::detail::has_common_type<T, objc::types::ID>
            template <typename Target>
            using is_object_pointer = has_common_type<std::decay_t<Target>, types::ID>;
        
        } /* namespace detail */
        
        /// Unnecessarily overwrought compile-time test for objc::message and descendants
        
        template <typename T>
        struct is_argument_list : decltype(detail::test_is_argument_list<T>(0)) {
            template <typename X = std::enable_if<decltype(detail::test_is_argument_list<T>(0))::value>>
            static constexpr bool value() { return true; }
            static constexpr bool value() {
                static_assert(decltype(detail::test_is_argument_list<T>(0))::value,
                              "Type does not conform to objc::arguments<Args...>");
                return detail::test_is_argument_list<T>(0);
            }
        };
        
        /// compile-time tests for objective-c primitives:
        
        /// test for an object-pointer instance (NSObject* and descendants)
        /// ... uses std::is_pointer<T> to enable_if itself properly,
        /// ... and objc::traits::detail::is_object_pointer<T> to make the call
        template <typename T, typename V = bool>
        struct is_object : std::false_type {};
        template <typename T>
        struct is_object<T,
            typename std::enable_if_t<
                 std::is_pointer<T>::value,
                 bool>> : detail::is_object_pointer<T> {};
        
        /// test for a selector struct
        template <typename T, typename V = bool>
        struct is_selector : std::false_type {};
        template <typename T>
        struct is_selector<T,
            typename std::enable_if_t<
                std::is_same<T, objc::types::selector>::value,
                bool>> : std::true_type {};
        
        /// test for the objective-c class struct type
        template <typename T, typename V = bool>
        struct is_class : std::false_type {};
        template <typename T>
        struct is_class<T,
            typename std::enable_if_t<
                detail::has_superclass<T>::value,
                bool>> : std::true_type {};
        
    } /* namespace traits */
    
} /* namespace objc */


#endif /// LIBIMREAD_OBJC_RT_TRAITS_HH