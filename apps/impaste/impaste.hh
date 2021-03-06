/// Copyright 2012-2017 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef APPS_IMPASTE_HH_
#define APPS_IMPASTE_HH_

#include <string>
#include <atomic>
#include <subjective-c/subjective-c.hpp>
#include <libimread/errors.hh>

#import  <Foundation/Foundation.h>
#import  <AppKit/AppKit.h>
#import  <subjective-c/categories/NSDictionary+IM.hh>

/// printffery
#define IMPASTE_DEBUG 0

/// return-value and verbosity-level static globals
extern std::atomic<int>     return_value;
extern std::atomic<int>     verbosity;

/// App delegate only implements `applicationWillTerminate:`
@interface AXAppDelegate : NSObject <NSApplicationDelegate> {}
- (void) applicationShouldTerminate:(NSApplication*)sender;
@end

/// Base thread class with options
@interface AXThread : NSThread {}
@property (atomic, strong) NSDictionary* options;
- (instancetype) initWithOptions:(NSDictionary*)optionsDict;
@end

/// a shortcut macro for exiting an AXThread “- main” method:
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

@interface AXImageCopyAndSaveThread : AXThread {}
- (void) main;
@end

namespace objc {
    
    /// function templated on an Objective-C type, for the
    /// execution of an AXThread subclass with a proper app delegate:
    
    template <typename OCThreadType> inline
    void run_thread(NSDictionary* options = @{}) {
        @autoreleasepool {
            // WTF("ABOUT TO INITIALIZE");
            [NSApplication sharedApplication];
            // WTF("ABOUT TO RUN THREAD");
            [[[OCThreadType alloc] initWithOptions:options] start];
            AXAppDelegate* delegate = [[AXAppDelegate alloc] init];
            NSApp.delegate = delegate;
            [NSApp run];
        };
    }
    
    /// OBJC_VERSION mapped to objc::config::version --
    /// a holdover from subjective-c-config:
    
    namespace config {
        const std::string version = OBJC_VERSION;
    }
    
}

#endif /// APPS_IMPASTE_HH_