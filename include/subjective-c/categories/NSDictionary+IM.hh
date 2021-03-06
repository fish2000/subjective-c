/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_

#include <subjective-c/subjective-c.hpp>
#include <libimread/options.hh>
#import  <Foundation/Foundation.h>

using OptionsMap = im::Options;

@interface NSDictionary (AXDictionaryAdditions)
+ (instancetype) dictionaryWithOptionsMap:(OptionsMap const&)optionsMap;
-                initWithOptionsMap:(OptionsMap const&)optionsMap;
- (OptionsMap)   asOptionsMap;
@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSDICTIONARY_PLUS_IM_HH_