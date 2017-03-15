/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef APPS_IMPASTE_HH_
#define APPS_IMPASTE_HH_

#include <string>
#include <atomic>
#include <type_traits>
#include <subjective-c/subjective-c.hpp>
#import  <Foundation/Foundation.h>
#import  <AppKit/AppKit.h>
#import  <subjective-c/categories/NSString+STL.hh>
#import  <subjective-c/categories/NSURL+IM.hh>
#import  <subjective-c/categories/NSBitmapImageRep+IM.hh>
#import  <subjective-c/categories/NSDictionary+IM.hh>
#import  <subjective-c/categories/NSData+IM.hh>
#include <libimread/ext/filesystem/path.h>

/// printffery
#define IMPASTE_DEBUG 1

/// return-value and verbosity-level static globals
extern std::atomic<int>     return_value;
extern std::atomic<int>     verbosity;

/// App delegate only implements `applicationWillTerminate:`
@interface AXAppDelegate : NSObject <NSApplicationDelegate> {}
- (void) applicationWillTerminate:(NSApplication*)application;
@end

/// Base thread class with options
@interface AXThread : NSThread {}
@property (atomic, strong) NSDictionary* options;
- (instancetype) initWithOptions:(NSDictionary*)optionsDict;
@end

#define AXTHREADEXIT(exitval)                                                   \
    return_value.store(exitval);                                                \
    [NSApp terminate:self];                                                     \
    return;


/// NSThread declarations and definitions,
/// one(ish) for each CLI option

@interface AXCheckThread : AXThread {}
- (void) main;
@end

@interface AXDryRunThread : AXThread {}
- (void) main;
@end

@interface AXImageCopyThread : AXThread {}
- (void) main;
@end

@interface AXImageSaveThread : AXThread {}
- (void) main;
@end

@interface AXImageCopyAndSaveThread : AXThread {}
- (void) main;
@end

namespace objc {
    
    template <typename OCThreadType> inline
    void run_thread(NSDictionary* options = @{}) {
        @autoreleasepool {
            [NSApplication sharedApplication];
            [[[OCThreadType alloc] initWithOptions:options] start];
            AXAppDelegate* delegate = [[AXAppDelegate alloc] init];
            NSApp.delegate = delegate;
            [NSApp run];
        };
    }
    
}


namespace objc {
    
    namespace config {
        
        const std::string version = OBJC_VERSION;
        
    };
    
};


#endif /// APPS_IMPASTE_HH_