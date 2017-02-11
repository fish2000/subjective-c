/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef SUBJECTIVE_C_SELECTOR_HH
#define SUBJECTIVE_C_SELECTOR_HH

#include <sstream>
#include <string>
#include "types.hh"

namespace objc {
    
    /// Straightforward wrapper around an objective-c selector (the SEL type).
    /// + Constructable from, and convertable to, common string types
    /// + Overloaded for equality testing
    
    struct selector {
        
        types::selector sel;
        
        explicit selector(std::string const& name);
        explicit selector(char const* name);
        explicit selector(NSString* name);
        
        selector(types::selector s);
        selector(objc::selector const& other);
        selector(objc::selector&& other) noexcept;
        
        objc::selector& operator=(const objc::selector& other);
        objc::selector& operator=(types::selector other);
        
        bool operator==(objc::selector const& s) const;
        bool operator!=(objc::selector const& s) const;
        bool operator==(types::selector const& s) const;
        bool operator!=(types::selector const& s) const;
        
        char const* c_str() const;
        std::string str() const;
        NSString* ns_str() const;
        CFStringRef cf_str() const;
        
        friend std::ostream& operator<<(std::ostream& os, objc::selector const& s) {
            return os << "@selector( " << s.str() << " )";
        }
        
        std::size_t hash() const;
        void swap(objc::selector& other) noexcept;
        void swap(types::selector& other) noexcept;
        
        operator types::selector() const;
        operator std::string() const;
        operator char const*() const;
        operator char*() const;
        operator NSString*() const;
        operator CFStringRef() const;
        
        static objc::selector register_name(std::string const& name) {
            return objc::selector(name);
        }
        static objc::selector register_name(char const* name) {
            return objc::selector(name);
        }
        static objc::selector register_name(NSString* name) {
            return objc::selector(name);
        }
        
        private:
            selector(void);
        
    };
    
} /* namespace objc */

/// string suffix for inline declaration of objc::selector objects
/// ... e.g. create an inline wrapper for a `yoDogg:` selector like so:
///     objc::selector yodogg = "yoDogg:"_SEL;

objc::selector operator"" _SEL(char const* name);

#endif /// SUBJECTIVE_C_SELECTOR_HH