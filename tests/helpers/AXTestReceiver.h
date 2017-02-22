
#import <Foundation/Foundation.h>

typedef struct struct_return {
    int value;
} StructReturn;

@interface AXTestReceiver : NSObject {}

+ (void)            callStatic;
+ (void)            callStaticWithInt:(int)arg;
+ (void)            callStaticWithInt:(int)arg
                        andObjCString:(NSString*)anotherArg;

- (instancetype)    callMethod;
- (instancetype)    callMethodWithInt:(int)arg;
- (instancetype)    callMethodWithInt:(int)arg
                        andObjCString:(NSString*)anotherArg;
- (instancetype)    callMethodWithInt:(int)arg
                       andVoidPointer:(void*)anotherVoidArg;

- (float)           returnFloat;
- (StructReturn)    returnStruct;

@end