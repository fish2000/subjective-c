
#import "BlockFptr.h"

void* BlockFptr(id block) {
    @synchronized (block) {
        MABlockClosure* closure = objc_getAssociatedObject(block, BlockFptr);
        if (!closure) {
            closure = [[MABlockClosure alloc] initWithBlock:block];
            objc_setAssociatedObject(block, BlockFptr, closure, OBJC_ASSOCIATION_RETAIN);
            #if !__has_feature(objc_arc)
            [closure release]; // retained by the associated object assignment
            #endif
        }
        // #if !__has_feature(objc_arc)
        return [closure fptr];
        // #endif
    }
}

void* BlockFptrAuto(id block) {
    #if !__has_feature(objc_arc)
    return BlockFptr([[block copy] autorelease]);
    #else
    return BlockFptr([block copy]);
    #endif
}
