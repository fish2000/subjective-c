/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_MEMORY_HH_
#define LIBIMREAD_MEMORY_HH_

#include <cstdio>
#include <vector>
#include <memory>

#include <libimread/libimread.hpp>
#include <libimread/ext/memory/fmemopen.hh>
#include <libimread/seekable.hh>

namespace im {
    
    class memory_source : public byte_source {
        public:
            memory_source(const byte *c, const int l)
                :data(c), len(l), pos(0)
                { }
            
            virtual ~memory_source() { }
            
            virtual std::size_t read(byte *buffer, std::size_t n) {
                if (pos + n > len) { n = len-pos; }
                /// FYI, std::memmove() actually copies bytes, rather
                /// than 'moving' them (whatever that might mean)
                std::memmove(buffer, data + pos, n);
                pos += n;
                return n;
            }
            
            virtual bool can_seek() const noexcept { return true; }
            virtual std::size_t seek_absolute(std::size_t p) { return pos = p; }
            virtual std::size_t seek_relative(int delta) { return pos += delta; }
            virtual std::size_t seek_end(int delta) { return pos = (len-delta-1); }
        
        private:
            const byte *data;
            const std::size_t len;
            std::size_t pos;
    };
    
    class memory_sink : public byte_sink {
        public:
            memory_sink(byte *c, std::size_t l)
                :data(c), membuf(memory::sink(data, l)), len(l)
                {}
            
            virtual ~memory_sink() {}
            
            virtual bool can_seek() const noexcept { return true; }
            virtual std::size_t seek_absolute(std::size_t pos) { return ::fseek(membuf.get(), pos, SEEK_SET); }
            virtual std::size_t seek_relative(int delta) { return ::fseek(membuf.get(), delta, SEEK_CUR); }
            virtual std::size_t seek_end(int delta) { return ::fseek(membuf.get(), delta, SEEK_END); }
            
            virtual std::size_t write(const void *buffer, std::size_t n) {
                return ::fwrite(buffer, sizeof(byte), n, membuf.get());
            }
            
            virtual void flush() { ::fflush(membuf.get()); }
            
            virtual std::vector<byte> contents() {
                std::vector<byte> out(len);
                std::memcpy(&out[0], data, out.size());
                return out;
            }
            
        private:
            byte *data;
            memory::buffer membuf;
            const std::size_t len;
    };

}

#endif /// LIBIMREAD_MEMORY_HH_
