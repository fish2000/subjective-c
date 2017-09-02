
#import  <subjective-c/categories/NSString+STL.hh>
#import  <subjective-c/categories/NSDictionary+IM.hh>
#include <libimread/options.hh>
#include "include/catch.hpp"

namespace {
    
    using im::Options;
    using NSStringArray = NSArray<NSString*>;
    
    TEST_CASE("[nsdictionary-options-map] Convert NSDictionary to im::Options",
              "[nsdictionary-options-map-convert-nsdictionary-to-im-options]")
    {
        @autoreleasepool {
            
            NSDictionary* dict = @{
                @"one" :        @1,
                @"two" :        @2.0,
                @"three" :      @[
                                    @875, @8765, @97764
                                ],
                @"four" :       @{
                                    @"yo"    : @"dogg",
                                    @"i"     : @"heard",
                                    @"you"   : @"like",
                                    @"dicts" : @"IN YOUR DICTS"
                                }
            };
            
            Options opts = [dict asOptionsMap];
            
            CHECK(opts.cast<int>("one")         == 1);
            CHECK(opts.cast<float>("two")       == 2.0f);
            
            CHECK(int(opts["three"][0])         == 875);
            CHECK(int(opts["three"][1])         == 8765);
            CHECK(int(opts["three"][2])         == 97764);
            
            CHECK(opts.subgroup("four").get("yo")    == "dogg");
            CHECK(opts.subgroup("four").get("i")     == "heard");
            CHECK(opts.subgroup("four").get("you")   == "like");
            CHECK(opts.subgroup("four").get("dicts") == "IN YOUR DICTS");
            
        }
    }
    
    
    TEST_CASE("[nsdictionary-options-map] Convert im::Options to NSDictionary",
              "[nsdictionary-options-map-convert-im-options-to-nsdictionary]")
    {
        @autoreleasepool {
            
            Options opts;
            
            opts.set("one", 11);
            opts.set("two", 222.22f);
            opts.set("three", Json{ 33, 333, 3333 });
            opts.set("four", Options());
            opts["four"]["yo"] = "dogg";
            opts["four"]["i"] = "heard";
            opts["four"]["you"] = "like";
            opts["four"]["dicts"] = "IN YOUR DICTS IN YOUR DICTS!!";
            
            NSDictionary* dict = [[NSDictionary alloc] initWithOptionsMap:opts];
            
            CHECK([(NSNumber*)dict[@"one"]              isEqual:@11]);
            CHECK([(NSNumber*)dict[@"two"]              isEqual:@222.22]);
            CHECK([(NSNumber*)dict[@"three"][0]         isEqual:@33]);
            CHECK([(NSNumber*)dict[@"three"][1]         isEqual:@333]);
            CHECK([(NSNumber*)dict[@"three"][2]         isEqual:@3333]);
            
            // WTF("Inner dict value ('four'):",
            //     [dict[@"four"][@"yo"] STLString],
            //     [dict[@"four"][@"i"] STLString],
            //     [dict[@"four"][@"you"] STLString],
            //     [dict[@"four"][@"dicts"] STLString]);
            
            NSStringArray* keys = [dict[@"four"] allKeys];
            
            // for (int idx = 0; idx < keys.count; idx++) {
            //     WTF("Key value for 'four':", [keys[idx] STLString]);
            // }
            
            CHECK([(NSString*)dict[@"four"][@"yo"]      isEqual:@"dogg"]);
            CHECK([(NSString*)dict[@"four"][@"i"]       isEqual:@"heard"]);
            CHECK([(NSString*)dict[@"four"][@"you"]     isEqual:@"like"]);
            CHECK([(NSString*)dict[@"four"][@"dicts"]   isEqual:@"IN YOUR DICTS IN YOUR DICTS!!"]);
            
        }
    }
    
    
}

