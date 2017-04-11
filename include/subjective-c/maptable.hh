/// Copyright 2012-2017 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef SUBJECTIVE_C_MAPTABLE_HH
#define SUBJECTIVE_C_MAPTABLE_HH

#include <memory>
#include <libimread/store.hh>
#import  <Foundation/NSMapTable.h>

namespace objc {
    
    using maptable_ptr = std::unique_ptr<NSMapTable, decltype(NSFreeMapTable)&>;
    
    class maptable : public store::stringmapper {
        
        public:
            DECLARE_STRINGMAPPER_TEMPLATE_CONSTRUCTORS(maptable);
        
        public:
            virtual bool can_store() const noexcept override;
        
        public:
            maptable(void);
            maptable(maptable&&) noexcept;
            virtual ~maptable();
        
        protected:
            bool has(std::string const&) const;
            bool has(NSString*) const;
        
        public:
            std::string& get_force(std::string const&) const;
        
        public:
            /// implementation of the stringmapper API, in terms of the RocksDB API
            virtual std::string&       get(std::string const& key) override;
            virtual std::string const& get(std::string const& key) const override;
            virtual bool set(std::string const& key, std::string const& value) override;
            virtual bool del(std::string const& key) override;
            virtual std::size_t count() const override;
            virtual stringvec_t list() const override;
        
        protected:
            mutable maptable_ptr instance{ nullptr, NSFreeMapTable };
    };
    
} /// namespace store

#endif /// SUBJECTIVE_C_MAPTABLE_HH