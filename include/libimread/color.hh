/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_COLOR_HH_
#define LIBIMREAD_COLOR_HH_

#include <cstdint>
#include <iomanip>
#include <utility>
#include <limits>
#include <array>
#include <set>
#include <bitset>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>

#ifndef ALIGN_AS
#define ALIGN_AS(type) __attribute__((aligned (alignof(type))))
#endif

namespace im {
    
    namespace detail {
        
        /// to_hex() courtesy of:
        /// http://stackoverflow.com/a/5100745/298171
        template <typename T> inline
        std::string to_hex(T tvalue) {
            std::stringstream stream;
            stream << "0x"
                   << std::setfill('0') << std::setw(sizeof(T) * 2)
                   << std::hex << tvalue;
            return stream.str();
        }
        
        template <typename T, typename U>
        using same = typename std::is_same<T, U>::type;
        
        template <typename T, typename U>
        using different = typename std::integral_constant<bool, !std::is_same<T, U>::value>::type;
        
    }
    
    namespace meta {
        template <typename Channel>
        struct ChannelBase {
            using Limits = std::numeric_limits<Channel>;
            using is_value = std::true_type;
            static constexpr Channel min() noexcept { return Limits::min(); }
            static constexpr Channel max() noexcept { return Limits::max(); }
            static constexpr Channel infinity() noexcept { return Limits::infinity(); }
            static constexpr bool is_fp() noexcept  { return std::is_floating_point<Channel>::value; }
            static constexpr bool is_int() noexcept { return std::is_integral<Channel>::value; }
            static constexpr bool is_signed() noexcept { return std::is_signed<Channel>::value; }
            static constexpr bool contains_value() noexcept { return true; }
        };
        
        template <typename Base>
        struct NonValueBase : public Base {
            using is_value = std::false_type;
            enum channels : typename Base::channel_t { None };
            static constexpr uint8_t channel_count = 0;
            static constexpr bool is_fp() noexcept { return false; }
            static constexpr bool is_int() noexcept { return false; }
            static constexpr bool is_signed() noexcept { return false; }
            static constexpr bool contains_value() noexcept { return false; }
            // static constexpr bool check() { return false; }
            // static constexpr bool check(const NonValueBase& b) { return false; }
            // static constexpr bool check(const Base& b) { return typename Base::is_value(); }
        };
        
        template <typename Channel>
        struct Mono : public ChannelBase<Channel> {
            enum channels : Channel { Y, None };
            static constexpr uint8_t channel_count = 1;
        };
        
        template <typename Channel>
        struct RGB : public ChannelBase<Channel> {
            enum channels : Channel { R, G, B, None };
            static constexpr uint8_t channel_count = 3;
        };
        
        template <typename Channel>
        struct BGR : public ChannelBase<Channel> {
            enum channels : Channel { B, G, R, None };
            static constexpr uint8_t channel_count = 3;
        };
        
        template <typename Channel>
        struct RGBA : public ChannelBase<Channel> {
            enum channels : Channel { R, G, B, A, None };
            static constexpr uint8_t channel_count = 4;
        };
    }
    
    template <template <typename> class ChannelMeta = meta::RGBA,
              typename Composite = uint32_t,
              typename Channel = uint8_t>
    struct alignas(Composite) UniformColor : public ChannelMeta<Channel> {
        
        static_assert(sizeof(Composite) > sizeof(Channel),
                     "UniformColor needs a composite type larger than its channel type");
        
        static constexpr uint8_t N = ChannelMeta<Channel>::channel_count;
        using Meta = ChannelMeta<Channel>;
        using NonValue = meta::NonValueBase<UniformColor<ChannelMeta, Composite, Channel>>;
        
        /// WHY U NO ALIGN MY ALIASED ARRAY TYPES CLAAAAAAAAAAAAANG
        /// ... makin me use stupid macros you stupid jerkface
        //using Components = alignas(Composite) Channel[N];
        using Components = ALIGN_AS(Composite) Channel[N];
        
        using bitset_t = std::bitset<sizeof(Composite) * 8>;
        using array_t = std::array<Channel, N>;
        using index_t = std::make_index_sequence<N>;
        using channel_list_t = std::initializer_list<Channel>;
        using component_t = Components;
        using composite_t = Composite;
        using channel_t = Channel;
        
        union alignas(Composite) {
            Components components;
            Composite composite{ 0 };
        };
        
        constexpr UniformColor() noexcept = default;
        
        explicit constexpr UniformColor(const Composite c) noexcept
            :composite(c)
            {}
        
        explicit constexpr UniformColor(const Components c) noexcept {
            component_assign_impl(c, index_t());
        }
        
        explicit constexpr UniformColor(channel_list_t initlist) noexcept {
            int idx = 0;
            for (auto it = initlist.begin();
                 it != initlist.end() && idx < N;
                 ++it) { components[idx] = *it;
                         ++idx; }
        }
        
        explicit constexpr UniformColor(array_t array) noexcept {
            int idx = 0;
            for (auto it = array.begin();
                 it != array.end() && idx < N;
                 ++it) { components[idx] = array[idx];
                         ++idx; }
        }
        
        constexpr operator Composite() const noexcept { return composite; }
        constexpr operator array_t() const noexcept { return array_impl(index_t()); }
        const operator std::string() const { return string_impl(index_t()); }
        
        constexpr Channel &operator[](std::size_t c) noexcept { return components[c]; }
        
        template <typename Color>
        constexpr bool operator<(const Color& rhs) const noexcept { return bool(composite < rhs.composite); }
        template <typename Color>
        constexpr bool operator>(const Color& rhs) const noexcept { return bool(composite > rhs.composite); }
        template <typename Color>
        constexpr bool operator<=(const Color& rhs) const noexcept { return bool(composite <= rhs.composite); }
        template <typename Color>
        constexpr bool operator>=(const Color& rhs) const noexcept { return bool(composite >= rhs.composite); }
        template <typename Color>
        constexpr bool operator==(const Color& rhs) const noexcept { return bool(composite == rhs.composite); }
        template <typename Color>
        constexpr bool operator!=(const Color& rhs) const noexcept { return bool(composite != rhs.composite); }
        
        constexpr bool operator<(const Composite& rhs) noexcept { return bool(composite < rhs); }
        constexpr bool operator>(const Composite& rhs) noexcept { return bool(composite > rhs); }
        constexpr bool operator<=(const Composite& rhs) noexcept { return bool(composite <= rhs); }
        constexpr bool operator>=(const Composite& rhs) noexcept { return bool(composite >= rhs); }
        constexpr bool operator==(const Composite& rhs) noexcept { return bool(composite == rhs); }
        constexpr bool operator!=(const Composite& rhs) noexcept { return bool(composite != rhs); }
        
        constexpr bool operator<(const Components& rhs) noexcept { return bool(components < rhs); }
        constexpr bool operator>(const Components& rhs) noexcept { return bool(components > rhs); }
        constexpr bool operator<=(const Components& rhs) noexcept { return bool(components <= rhs); }
        constexpr bool operator>=(const Components& rhs) noexcept { return bool(components >= rhs); }
        constexpr bool operator==(const Components& rhs) noexcept { return bool(components == rhs); }
        constexpr bool operator!=(const Components& rhs) noexcept { return bool(components != rhs); }
        
        static constexpr std::size_t channels() noexcept { return N; }
        
        constexpr unsigned int distance(const UniformColor& rhs) const noexcept {
            return distance_impl(rhs, 0, index_t());
        }
        
        const std::string to_string() const {
            std::ostringstream stream;
            stream <<  "(" << detail::to_hex(composite)
                   << ") " << string_impl(index_t())
                   << "";
            return stream.str();
        }
        
        const array_t to_array() const {
            return array_impl(index_t());
        }
        
        private:
            template <std::size_t ...I> inline
            unsigned int distance_impl(const UniformColor& rhs,
                                       unsigned int out,
                                       std::index_sequence<I...>) const noexcept {
                out += std::abs(std::get<I...>(components) -
                                std::get<I...>(rhs.components));
                return out;
            }
            
            template <std::size_t ...I> inline
            void component_assign_impl(const Components c,
                                          std::index_sequence<I...>) const noexcept {
                unpack { (components[I] = c[I])... };
            }
            
            template <std::size_t ...I> inline
            array_t array_impl(std::index_sequence<I...>) const noexcept {
                return array_t{ components[I]... };
            }
            
            template <std::size_t ...I> inline
            std::string string_impl(std::index_sequence<I...>) const {
                std::string out("{ ");
                unpack {
                    (out += std::to_string(components[I]) +
                               (I == N-1 ? "" : ", "), 0)...
                };
                out += " }";
                return out;
            }
    };
    
    
    #define STATIC_ASSERT_SAME()                                                    \
        static_assert(detail::same<color_t, dest_color_t>(),                        \
                     "Color types color_t and dest_color_t must be the same");
    
    #define STATIC_ASSERT_DIFFERENT()                                               \
        static_assert(detail::different<color_t, dest_color_t>(),                   \
                     "Color types color_t and dest_color_t cannot be the same");
    
    #define DECLARE_CONVERTER(Color, DestColor)                                     \
        template <>                                                                 \
        struct Convert<Color, DestColor> : public ConverterBase<Color, DestColor>
    
    #define CONVERTER_OP(argname)                                                   \
        inline dest_color_t operator()(const component_t& argname) const
    
    namespace color {
        using Monochrome = UniformColor<meta::Mono, uint16_t, uint8_t>;
        using RGBA = UniformColor<>;
        using RGB = UniformColor<meta::RGB>;
        using HDR = UniformColor<meta::RGBA, int64_t, int16_t>;
        
        //using rgba = UniformColor<>
        
        template <typename Color, typename DestColor>
        struct ConverterBase {
            using color_t = Color;
            using dest_color_t = DestColor;
            using component_t = typename color_t::component_t;
            using val_t = typename dest_color_t::channel_t;
            constexpr ConverterBase() noexcept = default;
            //virtual ~ConverterBase() {}
            //dest_color_t operator()(const color_t& color) const = 0;
        };
        
        template <typename Color, typename DestColor>
        struct Convert : public ConverterBase<Color, DestColor> {
            //using Base = ConverterBase<Color, DestColor>;
        };
        
        #define DECLARE_IDENTITY_CONVERTER(Color)                                   \
            DECLARE_CONVERTER(Color, Color) {                                       \
                STATIC_ASSERT_SAME();                                               \
                CONVERTER_OP(components) {                                          \
                    return dest_color_t(components);                                \
                }                                                                   \
            }
        
        DECLARE_IDENTITY_CONVERTER(RGB);
        DECLARE_IDENTITY_CONVERTER(RGBA);
        DECLARE_IDENTITY_CONVERTER(Monochrome);
        
        DECLARE_CONVERTER(RGB, RGBA) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// discard alpha for now
                dest_color_t out{ components[0],
                                  components[1],
                                  components[2] };
                return out;
            }
        };
        
        DECLARE_CONVERTER(RGBA, RGB) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// set alpha to zero for now
                dest_color_t out{ components[0],
                                  components[1],
                                  components[2],
                                  0x00 };
                return out;
            }
        };
        
        DECLARE_CONVERTER(RGB, Monochrome) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// ITU R-601.2 -- adapted from my own Python code here:
                /// https://github.com/fish2000/pylire/blob/master/pylire/process/grayscale.py#L6-L12
                dest_color_t out{ val_t(float(components[0]) * 299.0f / 1000.0f +
                                        float(components[1]) * 587.0f / 1000.0f +
                                        float(components[2]) * 114.0f / 1000.0f) };
                return out;
            }
        };
        
        DECLARE_CONVERTER(RGBA, Monochrome) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// ITU R-601.2, as above -- 
                /// only taking the RGB values, ignoring alpha
                dest_color_t out{ val_t(float(components[0]) * 299.0f / 1000.0f +
                                        float(components[1]) * 587.0f / 1000.0f +
                                        float(components[2]) * 114.0f / 1000.0f) };
                return out;
            }
        };
        
        DECLARE_CONVERTER(Monochrome, RGB) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// using the one value for the many (x3)
                const val_t value = val_t(components[0]);
                dest_color_t out{ value, value, value };
                return out;
            }
        };
        
        DECLARE_CONVERTER(Monochrome, RGBA) {
            STATIC_ASSERT_DIFFERENT();
            CONVERTER_OP(components) {
                /// using the one value for the many (x4)
                const val_t value = val_t(components[0]);
                dest_color_t out{ value, value, value, value };
                return out;
            }
        };
        
    }
    
    #undef STATIC_ASSERT_SAME
    #undef STATIC_ASSERT_DIFFERENT
    #undef DECLARE_CONVERTER
    #undef CONVERTER_OP
    #undef DECLARE_IDENTITY_CONVERTER
    
}

#ifdef ALIGN_AS
#undef ALIGN_AS
#endif

#endif /// LIBIMREAD_COLOR_HH_