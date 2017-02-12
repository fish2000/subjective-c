/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_

#import  <Foundation/Foundation.h>
#import  <Cocoa/Cocoa.h>
#include <libimread/libimread.hpp>
#include <libimread/options.hh>

using im::options_map;

@interface NSDictionary (AXDictionaryAdditions)
+ (instancetype) dictionaryWithOptionsMap:(options_map const&)optionsMap;
-                initWithOptionsMap:(options_map const&)optionsMap;
- (options_map)  asOptionsMap;
@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_