// Copyright 2012-2014 Luis Pedro Coelho <luis@luispedro.org>
// License: MIT (see COPYING.MIT file)

#ifndef LPC_FORMATS_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012
#define LPC_FORMATS_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012

#include <memory>

#include <libimread/libimread.hpp>
#include <libimread/base.hh>
#include <libimread/tools.hh>

namespace im {
    
    std::unique_ptr<ImageFormat> get_format(const char*);
    std::unique_ptr<ImageFormat> format_for_filename(const char*);
    std::unique_ptr<ImageFormat> format_for_filename(std::string&);
    std::unique_ptr<ImageFormat> format_for_filename(const std::string&);
    const char *magic_format(byte_source*);

}

#endif // LPC_FORMATS_H_INCLUDE_GUARD_WED_FEB__1_16_34_50_WET_2012
