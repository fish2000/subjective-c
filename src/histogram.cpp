/// Copyright 2016 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <cmath>
#include <numeric>
#include <libimread/ext/valarray.hh>
#include <libimread/histogram.hh>
#include <libimread/errors.hh>
#include <libimread/image.hh>
#include <libimread/rehash.hh>

namespace im {
    
    namespace detail {
        using rehasher_t = hash::rehasher<float>;
        static const float flinitial = 0.00000000;
    }
    
    Histogram::Histogram(Histogram::bytevec_t const& plane)
        :data(plane.data(), plane.size())
        ,histogram(std::cref(detail::flinitial), UCHAR_MAX)
        {
            std::for_each(std::begin(data), std::end(data),
                          [this](byte const& b) { histogram[b] += 1.0; });
        }
    
    Histogram::Histogram(Image const* image)
        :data((byte*)image->rowp(), image->size())
        ,histogram(std::cref(detail::flinitial), UCHAR_MAX)
        {
            std::for_each(std::begin(data), std::end(data),
                          [this](byte const& b) { histogram[b] += 1.0; });
        }
    
    Histogram::Histogram(Image const* image, int zidx)
        :Histogram(image->template plane<byte>(zidx))
        {}
    
    Histogram::~Histogram() {}
    
    Histogram::begin_t Histogram::begin() {
        return std::begin(histogram);
    }
    
    Histogram::end_t Histogram::end() {
        return std::end(histogram);
    }
    
    Histogram::const_begin_t Histogram::begin() const {
        return std::begin(histogram);
    }
    
    Histogram::const_end_t Histogram::end() const {
        return std::end(histogram);
    }
    
    std::size_t Histogram::size() const {
        return histogram.size();
    }
    
    float Histogram::sum() const {
        return histogram.sum();
    }
    
    float Histogram::min() const {
        return histogram.min();
    }
    
    float Histogram::max() const {
        return histogram.max();
    }
    
    int Histogram::min_value() const {
        /// the most infrequently occuring value
        auto result = std::find(std::begin(histogram),
                                std::end(histogram),
                                histogram.min());
        return result == std::end(histogram) ? -1 : *result;
    }
    
    int Histogram::max_value() const {
        /// the most frequently occuring value
        auto result = std::find(std::begin(histogram),
                                std::end(histogram),
                                histogram.max());
        return result == std::end(histogram) ? -1 : *result;
    }
    
    float Histogram::entropy() const {
        if (!entropy_calculated) {
            float histosize = 1.0 / histogram.sum();
            floatva_t histofloat = histogram * histosize;
            floatva_t divisor = histofloat.apply([](float d) -> float {
                float out = d * std::log(d);
                return std::isnan(out) ? 0.00 : out;
            });
            entropy_value = -divisor.sum();
            entropy_calculated = true;
        }
        return entropy_value;
    }
    
    Histogram::floatva_t Histogram::normalized() const {
        floatva_t out = histogram / histogram.max();
        return out;
    }
    
    Histogram::byteva_t const& Histogram::sourcedata() const {
        return data;
    }
    
    Histogram::floatva_t& Histogram::values() {
        return histogram;
    }
    
    Histogram::floatva_t const& Histogram::values() const {
        return histogram;
    }
    
    void Histogram::swap(Histogram& other) noexcept {
        using std::swap;
        swap(data,               other.data);
        swap(histogram,          other.histogram);
        swap(entropy_value,      other.entropy_value);
        swap(entropy_calculated, other.entropy_calculated);
    }
    
    void swap(im::Histogram& p0, im::Histogram& p1) noexcept {
        p0.swap(p1);
    }
    
    std::size_t Histogram::hash(std::size_t seed) const noexcept {
        return std::accumulate(std::begin(histogram),
                               std::end(histogram),
                               seed, detail::rehasher_t());
    }
    
}