/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <subjective-c/categories/NSData+IM.hh>
#include <subjective-c/categories/NSString+STL.hh>

namespace objc {
    
    datasource::datasource(NSData* d)
        :data(d), pos(0)
        {
            #if !__has_feature(objc_arc)
                [data retain];
            #endif
        }
    
    datasource::datasource(NSMutableData* d)
        :data([NSData dataWithData:d]), pos(0)
        {
            #if !__has_feature(objc_arc)
                [data retain];
            #endif
        }
    
    datasource::~datasource() {
        #if !__has_feature(objc_arc)
            [data release];
        #endif
    }
    
    std::size_t datasource::read(byte* buffer, std::size_t n) const {
        if (pos + n > data.length) { n = data.length-pos; }
        std::memmove(buffer, (byte*)data.bytes + pos, n);
        pos += n;
        return n;
    }
    
    bool datasource::can_seek() const noexcept { return true; }
    
    std::size_t datasource::seek_absolute(std::size_t p) {
        return pos = p;
    }
    
    std::size_t datasource::seek_relative(int delta) {
        return pos += delta;
    }
    
    std::size_t datasource::seek_end(int delta) {
        return pos = (data.length-delta-1);
    }
    
    bytevec_t datasource::full_data() {
        bytevec_t out(data.length);
        std::memcpy(&out[0], (byte*)data.bytes, out.size());
        return out;
    }
    
    std::size_t datasource::size() const { return data.length; }
    
    void* datasource::readmap(std::size_t pageoffset) const {
        byte* out = (byte*)data.bytes;
        if (pageoffset) {
            out += pageoffset * ::getpagesize();
        }
        return static_cast<void*>(out);
    }
    
    datasink::datasink(NSData* d)
        :data([NSMutableData dataWithData:d]), pos(0)
        {
            #if !__has_feature(objc_arc)
                [data retain];
            #endif
        }
    
    datasink::datasink(NSMutableData* d)
        :data(d), pos(0)
        {
            #if !__has_feature(objc_arc)
                [data retain];
            #endif
        }
    
    datasink::~datasink() {
        #if !__has_feature(objc_arc)
            [data release];
        #endif
    }
    
    bool datasink::can_seek() const noexcept { return true; }
    
    std::size_t datasink::seek_absolute(std::size_t p) {
        return pos = p;
    }
    
    std::size_t datasink::seek_relative(int delta) {
        return pos += delta;
    }
    
    std::size_t datasink::seek_end(int delta) {
        return pos = (data.length-delta-1);
    }
    
    std::size_t datasink::write(const void* buffer, std::size_t n) {
        if (pos + n > data.length) { n = data.length-pos; }
        std::memmove((byte*)data.mutableBytes + pos, (byte*)buffer, n);
        pos += n;
        return n;
    }
    
    bytevec_t datasink::contents() {
        bytevec_t out(data.length);
        std::memcpy(&out[0], (byte*)data.bytes, out.size());
        return out;
    }

}

@implementation NSData (AXDataAdditions)

+ (instancetype) dataWithByteVector:(bytevec_t const&)byteVector {
    return [[NSData alloc] initWithBytes:(const void*)&byteVector[0]
                                  length:(NSInteger)byteVector.size()];
}

+ (instancetype) dataWithSTLString:(std::string const&)string {
    return [[NSData alloc] initWithBytes:(const void*)string.data()
                                  length:(NSInteger)string.size()];
}

+ (instancetype) dataWithByteSource:(byte_source*)byteSource {
    return [NSData dataWithByteVector:byteSource->full_data()];
}

+ (instancetype) dataWithByteSource:(byte_source*)byteSource
                             length:(NSUInteger)bytes {
    std::unique_ptr<byte[]> buffer = std::make_unique<byte[]>(bytes);
    __attribute__((unused))
    int idx = byteSource->read(buffer.get(),
                               static_cast<std::size_t>(bytes));
    return [[NSData alloc] initWithBytes:(const void*)buffer.get()
                                  length:(NSInteger)bytes];
}

- initWithByteVector:(bytevec_t const&)byteVector {
    return [self initWithBytes:(const void*)&byteVector[0]
                        length:(NSInteger)byteVector.size()];
}

- initWithSTLString:(std::string const&)string {
    return [self initWithBytes:(const void*)string.data()
                        length:(NSInteger)string.size()];
}

- initWithByteSource:(byte_source*)byteSource {
    return [self initWithByteVector:byteSource->full_data()];
}

- initWithByteSource:(byte_source*)byteSource
              length:(NSUInteger)bytes {
    std::unique_ptr<byte[]> buffer = std::make_unique<byte[]>(bytes);
    __attribute__((unused))
    int idx = byteSource->read(buffer.get(),
                               static_cast<std::size_t>(bytes));
    return [self initWithBytes:(const void*)buffer.get()
                        length:(NSInteger)bytes];
}

- (NSUInteger) writeUsingByteSink:(byte_sink*)byteSink {
    std::size_t out = byteSink->write((byte*)self.bytes,
                    static_cast<std::size_t>(self.length));
    byteSink->flush();
    return static_cast<NSUInteger>(out);
}

- (NSUInteger) writeUsingByteSink:(byte_sink*)byteSink
                           length:(NSUInteger)bytes {
    std::size_t out = byteSink->write((byte*)self.bytes,
                         static_cast<std::size_t>(bytes));
    byteSink->flush();
    return static_cast<NSUInteger>(out);
}

- (std::unique_ptr<objc::datasource>) dataSource {
    return std::make_unique<objc::datasource>(self);
}

- (std::string) STLString {
    return std::string(static_cast<const char*>(self.bytes),
                       static_cast<std::size_t>(self.length));
}

@end

@implementation NSMutableData (AXMutableDataAdditions)

- (std::unique_ptr<objc::datasink>) dataSink {
    return std::make_unique<objc::datasink>(self);
}

@end
