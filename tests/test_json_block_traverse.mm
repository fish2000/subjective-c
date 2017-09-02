
#include <string>
#include <iostream>
#import  <BlockClosure.h>

#include <libimread/libimread.hpp>
#include <libimread/ext/JSON/json11.h>
#include <libimread/errors.hh>
#include <subjective-c/subjective-c.hpp>

#include "include/catch.hpp"

namespace {
    
    TEST_CASE("[json-block-traverse] Traverse JSON tree with a block literal via MABlockClosure",
              "[json-block-traverse-with-block-literal]")
    {
        using Node = Json::JSONNode;
        using fptr_t = std::add_pointer_t<void(const Node*, Type, NodeType)>;
        Json dict;
        dict["one"] = "one.";
        dict["two"] = "two.";
        dict["three"] = { 435, 345987, 238746, 21 };
        
        // objc::block<int> idx = 0;
        __block int idx = 0;
        
        id block = ^(void* node, Type jt, NodeType jnt) {
            FORSURE(FF("[%i] %s node found: ", idx, ((Node*)node)->typestr()));
            std::cerr << "\t\t";
            ((Node*)node)->print(std::cerr);
            std::cerr << std::endl << std::endl;
            idx++;
        };
        
        #if !__has_feature(objc_arc)
        [block retain];
        #endif
        
        std::cerr << std::endl << std::endl;
        std::cerr << "\tTraversing dict:" << std::endl;
        std::cerr << "\t\t" << dict;
        std::cerr << std::endl << std::endl;
        dict.traverse((fptr_t)BlockFptr(block));
        std::cerr << std::endl << std::endl;
        
        #if !__has_feature(objc_arc)
        [block release];
        #endif
    }
    
}

