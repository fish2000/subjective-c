/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <numeric>
#include <libimread/libimread.hpp>
#include <libimread/image.hh>
#include <libimread/histogram.hh>
#include <libimread/rehash.hh>

namespace im {
    
    void* Image::rowp() const {
        return this->rowp(0);
    }
    
    int Image::nbytes() const {
        const int bits = this->nbits();
        return (bits / 8) + bool(bits % 8);
    }
    
    int Image::dim_or(int dim, int default_value) const {
        if (dim >= this->ndims()) { return default_value; }
        return this->dim(dim);
    }
    
    int Image::stride_or(int dim, int default_value) const {
        if (dim >= this->ndims()) { return default_value; }
        return this->stride(dim);
    }
    
    int Image::width() const {
        return dim(0);
    }
    
    int Image::height() const {
        return dim(1);
    }
    
    int Image::planes() const {
        return dim_or(2);
    }
    
    int Image::size() const {
        return dim_or(0) * dim_or(1) * dim_or(2) * dim_or(3);
    }
    
    Histogram Image::histogram() const {
        return Histogram(this);
    }
    
    float Image::entropy() const {
        Histogram histo = Histogram(this);
        return histo.entropy();
    }
    
    ImageFactory::~ImageFactory() {}
    
    ImageWithMetadata::ImageWithMetadata()
        :meta("")
        {}
    ImageWithMetadata::ImageWithMetadata(std::string const& m)
        :meta(m)
        {}
    
    ImageWithMetadata::~ImageWithMetadata() {}
    
    std::string const& ImageWithMetadata::get_meta() const { return meta; }
    
    void ImageWithMetadata::set_meta(std::string const& m) { meta = m; }
}
