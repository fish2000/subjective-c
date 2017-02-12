/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSDATA_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSDATA_PLUS_IM_HH_

#include <memory>
#include <vector>
#import  <Foundation/Foundation.h>
#import  <Cocoa/Cocoa.h>
#include <libimread/libimread.hpp>
#include <libimread/seekable.hh>

namespace objc {
    
    using im::byte;
    
    class datasource : public im::byte_source {
        
        public:
            datasource(NSData* d);
            datasource(NSMutableData* d);
            virtual ~datasource();
            
            virtual std::size_t read(byte* buffer, std::size_t n);
            
            virtual bool can_seek() const noexcept;
            virtual std::size_t seek_absolute(std::size_t p);
            virtual std::size_t seek_relative(int delta);
            virtual std::size_t seek_end(int delta);
            
            virtual std::vector<byte> full_data();
            virtual std::size_t size() const;
            
            virtual void* readmap(std::size_t pageoffset = 0) const;
        
        private:
            NSData* data;
            std::size_t pos;
    
    };
    
    class datasink : public im::byte_sink {
        
        public:
            datasink(NSData* d);
            datasink(NSMutableData* d);
            virtual ~datasink();
            
            virtual bool can_seek() const noexcept;
            virtual std::size_t seek_absolute(std::size_t p);
            virtual std::size_t seek_relative(int delta);
            virtual std::size_t seek_end(int delta);
            
            virtual std::size_t write(const void* buffer, std::size_t n);
            virtual std::vector<byte> contents();
            
        private:
            NSMutableData* data;
            std::size_t pos;
    
    };

}

using im::byte;
using im::byte_source;
using im::byte_sink;
using objc::datasource;
using objc::datasink;

@interface NSData (AXDataAdditions)
+ (instancetype)        dataWithByteVector:(std::vector<byte> const&)byteVector;
+ (instancetype)        dataWithSTLString:(std::string const&)string;
+ (instancetype)        dataWithByteSource:(byte_source*)byteSource;
+ (instancetype)        dataWithByteSource:(byte_source*)byteSource
                                    length:(NSUInteger)bytes;
-                       initWithByteVector:(std::vector<byte> const&)byteVector;
-                       initWithSTLString:(std::string const&)string;
-                       initWithByteSource:(byte_source*)byteSource;
-                       initWithByteSource:(byte_source*)byteSource
                                    length:(NSUInteger)bytes;
- (NSUInteger)          writeUsingByteSink:(byte_sink*)byteSink;
- (NSUInteger)          writeUsingByteSink:(byte_sink*)byteSink
                                    length:(NSUInteger)bytes;
- (std::unique_ptr<objc::datasource>) dataSource;
- (std::string)         STLString;
@end

@interface NSMutableData (AXMutableDataAdditions)
- (std::unique_ptr<objc::datasink>) dataSink;
@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSDATA_PLUS_IM_HH_