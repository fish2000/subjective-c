/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <string>

#include <libimread/libimread.hpp>
#include <libimread/options.hh>

namespace im {
    
    std::string           get_optional_string(const options_map& opts,
                                              const std::string& key) {
        return opts.has(key) ? std::string(opts.get(key)) : std::string("");
    }
    
    const char           *get_optional_cstring(const options_map& opts,
                                               const std::string& key) {
        return get_optional_string(opts, key).c_str();
    }
    
    int                   get_optional_int(const options_map& opts,
                                           const std::string& key,
                                           const int default_value) {
        return opts.has(key) ? int(opts.get(key)) : default_value;
    }
    
    bool                  get_optional_bool(const options_map &opts,
                                            const std::string& key,
                                            const bool default_value) {
        return get_optional_int(opts, key,
                                static_cast<bool>(default_value));
    }
    
}
