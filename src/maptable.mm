/// Copyright 2017 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <subjective-c/maptable.hh>
#include <subjective-c/subjective-c.hpp>
#import  <subjective-c/categories/NSString+STL.hh>

#define STRINGNULL() store::stringmapper::base_t::null_value()

namespace objc {
    
    bool maptable::can_store() const noexcept { return true; }
    
    maptable::maptable()
        :instance{ NSCreateMapTable(NSObjectMapKeyCallBacks,
                                    NSObjectMapValueCallBacks,
                                    0), NSFreeMapTable }
        {}
    
    /// move constructor
    maptable::maptable(maptable&& other) noexcept
        :instance(std::move(other.instance))
        {}
    
    maptable::~maptable() {
        instance.reset(nullptr);
    }
    
    bool maptable::has(std::string const& key) const {
        return has([NSString stringWithSTLString:key]);
    }
    
    bool maptable::has(NSString* nskey) const {
        return objc::to_bool(NSMapMember(instance.get(),
                                         objc::bridge<const void*>(nskey),
                                         nullptr, nullptr));
    }
    
    std::string& maptable::get_force(std::string const& key) const {
        NSString* nskey = [[NSString alloc] initWithSTLString:key];
        if (has(nskey)) {
            NSString* nsval = objc::bridge<NSString*>(NSMapGet(instance.get(),
                              objc::bridge<const void*>(nskey)));
            cache[key] = [nsval STLString];
            return cache.at(key);
        }
        return STRINGNULL();
    }
    
    std::string& maptable::get(std::string const& key) {
        if (cache.find(key) != cache.end()) {
            return cache.at(key);
        }
        return get_force(key);
    }
    
    std::string const& maptable::get(std::string const& key) const {
        if (cache.find(key) != cache.end()) {
            return cache.at(key);
        }
        return get_force(key);
    }
    
    bool maptable::set(std::string const& key, std::string const& value) {
        if (value == STRINGNULL()) { return del(key); }
        NSString* nskey = [[NSString alloc] initWithSTLString:key];
        NSString* nsval = [[NSString alloc] initWithSTLString:value];
        NSMapInsert(instance.get(),
                    objc::bridge<const void*>(nskey),
                    objc::bridge<const void*>(nsval));
        cache[key] = value;
        return true;
    }
    
    bool maptable::del(std::string const& key) {
        if (cache.find(key) != cache.end()) {
            cache.erase(key);
        }
        NSString* nskey = [[NSString alloc] initWithSTLString:key];
        if (has(nskey)) {
            NSMapRemove(instance.get(),
                        objc::bridge<const void*>(nskey));
            return true;
        }
        return false;
    }
    
    std::size_t maptable::count() const {
        return static_cast<std::size_t>(NSCountMapTable(instance.get()));
    }
    
    store::stringmapper::stringvec_t maptable::list() const {
        store::stringmapper::stringvec_t out{};
        void* key;
        void* value;
        
        out.reserve(NSCountMapTable(instance.get()));
        NSMapEnumerator maperator = NSEnumerateMapTable(instance.get());
        
        while (NSNextMapEnumeratorPair(&maperator, &key, &value)) {
            NSString* nskey = objc::bridge<NSString*>(key);
            NSString* nsval = objc::bridge<NSString*>(value);
            std::string skey([nskey STLString]);
            out.emplace_back(skey);
            cache[skey] = [nsval STLString];
        }
        
        NSEndMapTableEnumeration(&maperator);
        return out;
    }
    
} /// namespace objc

#undef SELF