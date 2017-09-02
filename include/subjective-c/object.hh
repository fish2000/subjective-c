/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef SUBJECTIVE_C_OBJECT_HH
#define SUBJECTIVE_C_OBJECT_HH

#include <sstream>
#include <string>
#include "types.hh"
#include "selector.hh"
#include "message-args.hh"
#include "traits.hh"
#include "demangle.hh"

namespace objc {
    
    /// wrapper around an objective-c instance
    /// ... FEATURING:
    /// + automatic scoped memory management via RAII through MRC messages
    /// ... plus fine control through inlined retain/release/autorelease methods
    /// + access to wrapped object pointer via operator*()
    /// + boolean selector-response test via operator[](T t) e.g.
    ///
    ///     objc::object<NSYoDogg*> yodogg([[NSYoDogg alloc] init]);
    ///     if (yodogg["iHeardYouRespondTo:"]) {
    ///         [*yodogg iHeardYouRespondTo:argsInYourArgs];
    ///     }
    ///
    /// + convenience methods e.g. yodogg.classname(), yodogg.description(), yodogg.lookup() ...
    /// + inline bridging template e.g. void* asVoid = yodogg.bridge<void*>();
    /// + E-Z static methods for looking shit up in the runtime heiarchy
    
    template <typename OCType>
    struct object {
        
        #if __has_feature(objc_arc)
        using pointer_t = objc::ocpointer_t<OCType>;
        #else
        using pointer_t = objc::ocpointer_t<OCType> __unsafe_unretained;
        #endif
        using object_t = typename objc::octype_t<OCType>;
        
        static_assert(objc::traits::is_object<pointer_t>::value,
                      "objc::object<OCType> requires an Objective-C object type");
        
        struct cls_t {
            
            objc::types::cls selfcls;
            
            cls_t()
                :selfcls(::objc_lookUpClass(
                         runtime::nameof<OCType>()))
                {
                    if (selfcls == nil) {
                        selfcls = [NSObject class];
                    }
                }
            
            template <typename T,
                      typename X = typename objc::traits::is_object<
                                            objc::ocpointer_t<T>>::type>
            cls_t(objc::ocpointer_t<T> ocptr = nil)
                :selfcls([objc::octype_t<T> class])
                {}
            
            explicit cls_t(types::cls othercls)
                :selfcls(othercls)
                {}
            
            explicit cls_t(types::ID instance)
                :selfcls([instance class])
                {}
            
            explicit cls_t(std::string const& clsname)
                :selfcls(::objc_lookUpClass(clsname.c_str()))
                {}
            
            cls_t(cls_t const& other)
                :selfcls(other.selfcls)
                {}
            
            cls_t(cls_t&& other) noexcept
                :selfcls(std::move(other.selfcls))
                {}
            
            std::string name() const {
                return ::class_getName(selfcls);
            }
            
            std::string description() const {
                return [[selfcls description] UTF8String];
            }
            
            objc::types::cls& get() const {
                return selfcls;
            }
            
            std::string to_string() const {
                std::ostringstream os;
                os << "<" << name()        << "> "
                   << "(" << description() << ")";
                return os.str();
            }
            
            friend std::ostream& operator<<(std::ostream& os, cls_t const& friendly) {
                return os << friendly.to_string();
            }
            
            void swap(cls_t& other) noexcept {
                using std::swap;
                using objc::swap;
                swap(this->selfcls,  other.selfcls);
            }
            
            friend void swap(cls_t& lhs, cls_t& rhs) noexcept {
                using std::swap;
                using objc::swap;
                swap(lhs.selfcls, rhs.selfcls);
            }
        };
        
        pointer_t self;
        cls_t cls;
        
        explicit object(pointer_t ii)
            :self(ii)
            { retain(); }
        
        object(object const& other)
            :self(other.self)
            { retain(); }
        
        object(object&& other) noexcept
            :self(other.self)
            { retain(); }
        
        virtual ~object() { release(); }
        
        object& operator=(pointer_t other) {
            if ([self isEqual:other] == NO) {
                object(other).swap(*this);
            }
            return *this;
        }
        
        object& operator=(object const& other) {
            if ([self isEqual:other.self] == NO) {
                object(other).swap(*this);
            }
            return *this;
        }
        
        object& operator=(object&& other) noexcept {
            if ([self isEqual:other.self] == NO) {
                object(std::move(other)).swap(*this);
            }
            return *this;
        }
        
        operator pointer_t()   const { return self; }
        pointer_t operator*()  const { return self; }
        pointer_t operator->() const { return self; }
        
        bool operator==(object const& other) const {
            return objc::to_bool([self isEqual:other.self]);
        }
        bool operator!=(object const& other) const {
            return !objc::to_bool([self isEqual:other.self]);
        }
        bool operator==(pointer_t const& other) const {
            return objc::to_bool([self isEqual:other]);
        }
        bool operator!=(pointer_t const& other) const {
            return !objc::to_bool([self isEqual:other]);
        }
        
        template <typename T> inline
        T bridge() {            return objc::bridge<T>(self); }
        
        template <typename T> inline
        T bridgeretain() {      return objc::bridgeretain<T>(self); }
        
        template <typename T> inline
        T bridgetransfer() {    return objc::bridgetransfer<T>(self); }
        
        inline bool responds_to(types::selector s) const {
            return objc::to_bool([self respondsToSelector:s]);
        }
        
        #if !__has_feature(objc_arc)
            inline void retain() const      { [self retain]; }
            inline void release() const     { [self release]; }
            inline void autorelease() const { [self autorelease]; }
        #else
            inline void retain() const      {}
            inline void release() const     {}
            inline void autorelease() const {}
        #endif
        
        bool operator[](types::selector s) const       { return responds_to(s); }
        bool operator[](objc::selector const& s) const { return responds_to(s.sel); }
        bool operator[](char const* s) const           { return responds_to(::sel_registerName(s)); }
        bool operator[](std::string const& s) const    { return responds_to(::sel_registerName(s.c_str())); }
        bool operator[](NSString* s) const             { return responds_to(::NSSelectorFromString(s)); }
        bool operator[](CFStringRef s) const           { return responds_to(::NSSelectorFromString(
                                                                        objc::bridge<NSString*>(s))); }
        
        std::size_t hash() const {
            return static_cast<std::size_t>([self hash]);
        }
        
        types::cls getclass() const { return cls.get(); }
        std::string classname() const { return cls.name(); }
        std::string description() const { return cls.description(); }
        types::cls lookup() const { return cls.get(); }
        
        std::string to_string() const {
            std::ostringstream os;
            os << cls.to_string() << " "
               << "[" << std::hex << "0x"
                      << hash()
                      << std::dec << "]";
            return os.str();
        }
        
        friend std::ostream& operator<<(std::ostream& os, object const& friendly) {
            return os << friendly.to_string();
        }
        
        void swap(object& other) noexcept {
            using std::swap;
            using objc::swap;
            swap(this->self, other.self);
            swap(this->cls,  other.cls);
        }
        
        void swap(pointer_t& other) noexcept {
            using std::swap;
            using objc::swap;
            swap(this->self, other);
            this->cls = [self class];
        }
        
        private:
            object(void);
    };
    
    using id = objc::object<types::ID>;
    
} /* namespace objc */

#endif /// SUBJECTIVE_C_OBJECT_HH