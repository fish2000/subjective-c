/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef APPS_SUBJECTIVE_C_CONFIG_H_
#define APPS_SUBJECTIVE_C_CONFIG_H_

#include <subjective-c/subjective-c.hpp>
#include <libimread/ext/pystring.hh>

namespace objc {
    
    namespace config {
        
        const std::string prefix = std::string(OBJC_INSTALL_PREFIX);
        const std::string exec_prefix = prefix;
        const std::string includes = std::string("-I") + pystring::replace(OBJC_INCLUDE_DIRECTORIES, ";", " -I");
        const std::string libs = std::string(OBJC_LINK_LIBRARIES);
        const std::string cflags = includes + " " + std::string(OBJC_COMPILE_OPTIONS);
        const std::string ldflags = std::string(OBJC_LINK_FLAGS) + " " + libs;
        
        const std::string version = OBJC_VERSION;
        
    };
    
};


#endif /// APPS_SUBJECTIVE_C_CONFIG_H_