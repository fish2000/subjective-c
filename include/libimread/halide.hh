// Copyright 2012-2014 Luis Pedro Coelho <luis@luispedro.org>
// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_HALIDE_H_
#define LIBIMREAD_HALIDE_H_

#include <cstring>
#include <memory>
#include <utility>
#include <stdio.h>
#include <stdint.h>

#include <Halide.h>

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/base.hh>
#include <libimread/file.hh>
#include <libimread/formats.hh>
#include <libimread/tools.hh>

namespace im {
    
    template <typename T>
    using HalImage = Halide::Image<typename std::decay<T>::type>;
    using MetaImage = ImageWithMetadata;
    
    template <typename pT>
    class HybridImage : public HalImage<pT>, public Image, public MetaImage {
        public:
            void finalize() {}
            uint8_t *release(uint8_t *ptr) { return nullptr; }
            
            HybridImage()
                :HalImage<pT>(), Image(), MetaImage()
                {}
            
            HybridImage(int x, int y, int z, int w, const std::string &name="")
                :HalImage<pT>(x, y, z, w, name), Image(), MetaImage(name)
                {}
            
            HybridImage(int x, int y, int z, const std::string &name="")
                :HalImage<pT>(x, y, z, name), Image(), MetaImage(name)
                {}
            
            HybridImage(int x, int y, const std::string &name="")
                :HalImage<pT>(x, y, name), Image(), MetaImage(name)
                {}
            
            HybridImage(int x, const std::string &name="")
                :HalImage<pT>(x, name), Image(), MetaImage(name)
                {}
            
            virtual ~HybridImage() {}
            
            virtual int nbits() const override {
                /// elem_size is in BYTES, so:
                return sizeof(pT) * 8;
            }
            
            inline int nbytes() const {
                return sizeof(pT);
            }
            
            virtual int ndims() const override {
                return this->dimensions();
            }
            
            virtual int dim(int d) const override {
                return this->extent(d);
            }
            
            inline int rowp_stride() const {
                return this->channels() == 1 ? 0 : this->stride(1);
            }
            
            void *rowp(int r) override {
                /// WARNING: FREAKY POINTERMATH FOLLOWS
                pT *host = this->data();
                host += (r * rowp_stride());
                return static_cast<void *>(host);
            }
            
    };
    
#define xWIDTH d1
#define xHEIGHT d0
#define xDEPTH d2
    
    template <typename T>
    class HalideFactory : public ImageFactory {
        private:
            std::string *name;
        
        public:
            typedef T pixel_type;
        
            HalideFactory()
                :name(new std::string(""))
                {}
            HalideFactory(const std::string &n)
                :name(new std::string(n))
                {}
            
            virtual ~HalideFactory() { delete name; }
        
        protected:
            std::unique_ptr<Image> create(int nbits,
                                          int xHEIGHT, int xWIDTH, int xDEPTH,
                                          int d3, int d4) {
                return std::unique_ptr<Image>(
                    new HybridImage<T>(
                        xWIDTH, xHEIGHT, xDEPTH));
            }
    };

#undef xWIDTH
#undef xHEIGHT
#undef xDEPTH

    namespace halide {
        
        //static const options_map opts; /// not currently used when reading
        
        template <typename T = uint8_t>
        Halide::Image<T> read(const std::string &filename) {
            options_map opts;
            HalideFactory<T> factory(filename);
            std::unique_ptr<ImageFormat> format(get_format(split_filename(filename.c_str())));
            
            _ASSERT(format.get(), "[imread] Format is unknown to libimread\n");
            _ASSERT(format->can_read(), "[imread] Format is unreadable by libimread\n");
            
            std::unique_ptr<byte_source> input(new file_source_sink(filename));
            std::unique_ptr<Image> output = format->read(input.get(), &factory, opts);
            HybridImage<T> image = dynamic_cast<HybridImage<T>&>(*output);
            
            _ASSERT(image.data(), "[imread] No data!");
            _ASSERT(image.defined(), "[imread] Output image undefined!");
            
            // fprintf(stderr, "Returning image: %ix%ix%i\n",
            //     image.width(), image.height(), image.channels());
            
            return image;
        }
        
    }
    
}

#endif // LIBIMREAD_HALIDE_H_
