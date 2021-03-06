/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <string>
#include <subjective-c/categories/NSData+IM.hh>
#include <subjective-c/categories/NSString+STL.hh>
#include <subjective-c/categories/NSDictionary+IM.hh>
#include <libimread/errors.hh>

using OptionsMap = im::Options;

static const NSJSONReadingOptions readingOptions = static_cast<NSJSONReadingOptions>(0);
static const NSJSONWritingOptions writingOptions = static_cast<NSJSONWritingOptions>(0);

@implementation NSDictionary (AXDictionaryAdditions)

+ (instancetype) dictionaryWithOptionsMap:(OptionsMap const&)optionsMap {
    NSDictionary* optionsDict;
    NSError* error;
    NSData* optionsJSON = [[NSData alloc] initWithSTLString:optionsMap.format()];
    optionsDict = (NSDictionary*)[NSJSONSerialization JSONObjectWithData:optionsJSON
                                                                 options:readingOptions
                                                                   error:&error];
    imread_assert(optionsDict != nil,
                  "NSDictionary error in dictionaryWithOptionsMap:",
                  [error.localizedDescription STLString]);
    return optionsDict;
}

- initWithOptionsMap:(OptionsMap const&)optionsMap {
    NSDictionary* optionsDict;
    NSError* error;
    NSData* optionsJSON = [[NSData alloc] initWithSTLString:optionsMap.format()];
    optionsDict = (NSDictionary*)[NSJSONSerialization JSONObjectWithData:optionsJSON
                                                                 options:readingOptions
                                                                   error:&error];
    imread_assert(optionsDict != nil,
                  "NSDictionary error in initWithOptionsMap:",
                  [error.localizedDescription STLString]);
    return [self initWithDictionary:optionsDict];
}

- (OptionsMap) asOptionsMap {
    NSError* error;
    NSData* datum = [NSJSONSerialization dataWithJSONObject:self
                                                    options:writingOptions
                                                      error:&error];
    imread_assert(datum != nil,
                  "NSDictionary error in asOptionsMap",
                  [error.localizedDescription STLString]);
    return OptionsMap::parse([datum STLString]);
}

@end
