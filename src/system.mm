/// Copyright 2012-2017 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <cstdlib>
#include <cerrno>
#include <sys/sysctl.h>

#include <subjective-c/system.hh>

namespace objc {
    
    std::string getsysctl(std::string const& name) {
        char buffer[1024];
        std::size_t size = sizeof(buffer);
        int status = ::sysctlbyname(name.c_str(), buffer, &size, nullptr, 0);
        if (status != 0) { return ""; }
        return std::string(buffer, size);
    }
    
} /// namespace objc