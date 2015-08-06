/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <cstdlib>

#include <libimread/libimread.hpp>
#include <libimread/objc-rt.hh>

#ifdef __OBJC__

@implementation NSString (IMStringAdditions)
+ (NSString *) stringWithSTLString:(const std::string&)str {
    NSString *out = [[NSString alloc] initWithUTF8String:str.c_str()];
    return out;
}
+ (NSString *) stringWithSTLWideString:(const std::wstring&)wstr {
    unsigned siz = wstr.size() * sizeof(wchar_t);
    const char *bytes = reinterpret_cast<const char*>(wstr.data());
    NSString *out = [[NSString alloc] initWithBytes:bytes
                                             length:siz
                                           encoding:kSTLWideStringEncoding];
    return out;
}
- (std::string) STLString {
    return [self STLStringUsingEncoding:NSUTF8StringEncoding];
}
- (std::string) STLStringUsingEncoding:(NSStringEncoding)encoding {
    return [self cStringUsingEncoding:encoding];
}
- (std::wstring) STLWideString {
    NSData *enc = [self dataUsingEncoding:kSTLWideStringEncoding];
    return std::wstring(
        static_cast<const wchar_t*>([enc bytes]),
        static_cast<unsigned>([enc length]) / sizeof(wchar_t));
}
@end

#endif /// __OBJC__