/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_HASHING_HH_
#define LIBIMREAD_HASHING_HH_

#include <cmath>
#include <array>
#include <bitset>
#include <algorithm>
#include <functional>
#include <type_traits>

#include <libimread/libimread.hpp>
#include <libimread/image.hh>
#include <libimread/pixels.hh>

namespace detail {
    
    template <typename T> inline
    void rehash(std::size_t& seed, const T& v) {
        /// also cribbed from boost,
        /// via http://stackoverflow.com/a/23860042/298171
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    
}

namespace blockhash {
    
    namespace orig {
        
        int cmpint(const void *pa, const void *pb) noexcept;
        int cmpfloat(const void *pa, const void *pb) noexcept;
        float median(int *data, int n);
        float medianf(float *data, int n);
        char* bits_to_hexhash(int *bits, int nbits);
        
        void blockhash_quick(int bits, unsigned char *data,
                             int width, int height, int **hash);
        
        void blockhash(int bits, unsigned char *data,
                       int width, int height, int **hash);
    
    };
    
    using im::Image;
    using im::byte;
    
    namespace detail {
        
        template <std::size_t NN, typename Iterator>
        float median(Iterator begin, Iterator end) {
            using Type = typename std::remove_pointer_t<std::decay_t<Iterator>>;
            
            /// middle for odd-length, and "upper-middle" for even length
            std::array<Type, NN> local;
            std::copy(begin, end, local.begin());
            auto diff = local.end() - local.begin();
            auto middle = local.begin() + diff / 2;
            
            /// This function runs in O(n) on average
            std::nth_element(local.begin(), middle, local.end());
            
            if (diff % 2 != 0) {
                /// odd length
                return *middle;
            } else {
                /// even length -- the "lower middle" is the max of the lower half
                auto lower_middle = std::max_element(local.begin(), middle);
                return (*middle + *lower_middle) / 2.0f;
            }
        }
        
    };
    
    template <std::size_t N = 8>
    std::bitset<N*N> blockhash_quick(Image& image) {
        constexpr int           NN = N*N;
        constexpr int           NN2 = NN/2;
        constexpr int           NN4 = NN/4;
        const int               block_width = image.dim(0) / N;
        const int               block_height = image.dim(1) / N;
        int                     i, x, y, ix, iy, value;
        float                   m[4];
        std::bitset<NN>         out;
        std::array<int, NN>     blocks;
        im::pix::accessor<byte> at = image.access();
        
        for (y = 0; y < N; y++) {
            for (x = 0; x < N; x++) {
                value = 0;
                for (iy = 0; iy < block_height; iy++) {
                    for (ix = 0; ix < block_width; ix++) {
                        /// calculate offset
                        value += at((x * block_width + ix), (y * block_height + iy), 0)[0] +
                                 at((x * block_width + ix), (y * block_height + iy), 1)[0] +
                                 at((x * block_width + ix), (y * block_height + iy), 2)[0];
                    }
                }
                blocks[y * N + x] = value;
            }
        }
        
        for (i = 0; i < 4; i++) {
            auto ppbegin = &blocks[i*NN4];
            auto ppend = ppbegin + NN4;
            m[i] = detail::median<NN4>(ppbegin, ppend);
        }
        
        for (i = 0; i < NN; i++) {
            if (  ((blocks[i] < m[0]) && (i < NN4))
               || ((blocks[i] < m[1]) && (i >= NN4) && (i < NN2))
               || ((blocks[i] < m[2]) && (i >= NN2) && (i < NN4+NN2))
               || ((blocks[i] < m[3]) && (i >= NN2+NN4))
               )
            {
              out[i] = false;
            } else {
              out[i] = true;
            }
        }
        
        return out;
    }
    
    template <std::size_t N = 8>
    std::bitset<N*N> blockhash(Image& image) {
        constexpr int               NN = N*N;
        constexpr int               NN2 = NN/2;
        constexpr int               NN4 = NN/4;
        const float                 block_width = (float) image.dim(0) / (float) N;
        const float                 block_height = (float) image.dim(1) / (float) N;
        const int                   width = image.dim(0);
        const int                   height = image.dim(1);
        float                       m[4];
        float                       y_frac, y_int;
        float                       x_frac, x_int;
        float                       x_mod, y_mod, value;
        float                       weight_top, weight_bottom, weight_left, weight_right;
        int                         block_top, block_bottom, block_left, block_right;
        int                         i, x, y;
        std::bitset<NN>             out;
        std::array<float, NN>       blocks;
        im::pix::accessor<byte>     at = image.access();
        
        if (width % N == 0 && height % N == 0) {
            return blockhash_quick<N>(image);
        }
        
        for (y = 0; y < height; y++) {
            y_mod = std::fmod(y + 1, block_height);
            y_frac = std::modf(y_mod, &y_int);
            
            weight_top = (1 - y_frac);
            weight_bottom = y_frac;
            
            /// y_int will be 0 on bottom/right borders and on block boundaries
            if (y_int > 0 || (y + 1) == height) {
                block_top = block_bottom = (int)std::floor((float) y / block_height);
            } else {
                block_top = (int)std::floor((float) y / block_height);
                block_bottom = (int)std::ceil((float) y / block_height);
            }
            
            for (x = 0; x < width; x++) {
                x_mod = std::fmod(x + 1, block_width);
                x_frac = std::modf(x_mod, &x_int);
                weight_left = (1 - x_frac);
                weight_right = x_frac;
                
                /// x_int will be 0 on bottom/right borders and on block boundaries
                if (x_int > 0 || (x + 1) == width) {
                    block_left = block_right = (int)std::floor((float) x / block_width);
                } else {
                    block_left = (int)std::floor((float) x / block_width);
                    block_right = (int)std::ceil((float) x / block_width);
                }
                
                /// get value at coords
                value = (float) at(x, y, 0)[0] +
                        (float) at(x, y, 1)[0] +
                        (float) at(x, y, 2)[0];
                
                /// add weighted pixel value to relevant blocks
                blocks[block_top * N + block_left] += value * weight_top * weight_left;
                blocks[block_top * N + block_right] += value * weight_top * weight_right;
                blocks[block_bottom * N + block_left] += value * weight_bottom * weight_left;
                blocks[block_bottom * N + block_right] += value * weight_bottom * weight_right;
            }
        }
        
        for (i = 0; i < 4; i++) {
            auto ppbegin = &blocks[i*NN4];
            auto ppend = ppbegin + NN4;
            m[i] = detail::median<NN4>(ppbegin, ppend);
        }
        
        for (i = 0; i < NN; i++) {
            if (  ((blocks[i] < m[0]) && (i < NN4))
               || ((blocks[i] < m[1]) && (i >= NN4) && (i < NN2))
               || ((blocks[i] < m[2]) && (i >= NN2) && (i < NN4+NN2))
               || ((blocks[i] < m[3]) && (i >= NN2+NN4))
               )
            {
              out[i] = false;
            } else {
              out[i] = true;
            }
        }
        
        return out;
    }    
};


#endif /// LIBIMREAD_HASHING_HH_