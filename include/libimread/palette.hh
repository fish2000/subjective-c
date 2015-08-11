/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_PALETTE_HH_
#define LIBIMREAD_PALETTE_HH_

#include <cstdint>
#include <limits>
#include <bitset>
#include <array>
#include <set>
#include <utility>
#include <iostream>

#include <libimread/libimread.hpp>
#include <libimread/process/neuquant.hh>
#include <libimread/errors.hh>

#ifndef ALIGN_AS
#define ALIGN_AS(type) __attribute__((aligned (alignof(type))))
#endif

namespace im {
    
    template <typename Channel>
    struct ChannelBase {
        using Limits = std::numeric_limits<Channel>;
        static constexpr Channel min() { return Limits::min(); }
        static constexpr Channel max() { return Limits::max(); }
    };
    
    template <typename Channel>
    struct Mono : ChannelBase<Channel> {
        enum channels : Channel { Y, None };
        static constexpr uint8_t channel_count = 1;
    };
    
    template <typename Channel>
    struct RGB : ChannelBase<Channel> {
        enum channels : Channel { R, G, B, None };
        static constexpr uint8_t channel_count = 3;
    };
    
    template <typename Channel>
    struct BGR : ChannelBase<Channel> {
        enum channels : Channel { B, G, R, None };
        static constexpr uint8_t channel_count = 3;
    };
    
    template <typename Channel>
    struct RGBA : ChannelBase<Channel> {
        enum channels : Channel { R, G, B, A, None };
        static constexpr uint8_t channel_count = 4;
    };
    
    template <template <typename> class ChannelMeta = RGBA,
              typename Composite = uint32_t,
              typename Channel = uint8_t>
    struct UniformColor : public ChannelMeta<Channel> {
        
        static_assert(sizeof(Composite) > sizeof(Channel),
                      "UniformColor needs a composite type larger than its channel type");
        
        static constexpr uint8_t N = ChannelMeta<Channel>::channel_count;
        using Meta = ChannelMeta<Channel>;
        
        /// WHY U NO ALIGN MY ALIASED ARRAY TYPES CLAAAAAAAAAAAAANG
        /// ... makin me use stupid macros you stupid jerkface
        //using Components = alignas(Composite) Channel[N];
        using Components = ALIGN_AS(Composite) Channel[N];
        
        using bitset_t = std::bitset<sizeof(Composite) * 8>;
        using array_t = std::array<Channel, N>;
        using index_t = std::make_index_sequence<N>;
        using component_t = Components;
        using composite_t = Composite;
        using channel_t = Channel;
        
        union alignas(Composite) {
            Components components;
            Composite composite{ 0 };
        };
        
        constexpr UniformColor() noexcept = default;
        
        explicit constexpr UniformColor(Composite c) noexcept
            :composite(c)
            {}
        
        explicit constexpr UniformColor(std::initializer_list<Channel> initlist) noexcept {
            int idx = 0;
            for (auto it = initlist.begin();
                 it != initlist.end() && idx < N;
                 ++it) { components[0] = *it; ++idx; }
        }
        
        constexpr operator Composite() noexcept { return composite; }
        constexpr operator array_t() noexcept { return array_impl(index_t()); }
        const operator std::string() { return string_impl(index_t()); }
        
        constexpr Channel &operator[](std::size_t c) { return components[c]; }
        constexpr bool operator<(const UniformColor& rhs) noexcept { return composite < rhs.composite; }
        constexpr bool operator>(const UniformColor& rhs) noexcept { return composite > rhs.composite; }
        constexpr bool operator<=(const UniformColor& rhs) noexcept { return composite <= rhs.composite; }
        constexpr bool operator>=(const UniformColor& rhs) noexcept { return composite >= rhs.composite; }
        constexpr bool operator==(const UniformColor& rhs) noexcept { return composite == rhs.composite; }
        constexpr bool operator!=(const UniformColor& rhs) noexcept { return composite != rhs.composite; }
        
        private:
            template <std::size_t ...I> inline
            array_t array_impl(std::index_sequence<I...>) {
                return array_t{ components[I]... };
            }
            template <std::size_t ...I> inline
            std::string string_impl(std::index_sequence<I...>) {
                std::string out("{ ");
                int unpack[] __attribute__((unused)) { 0, 
                    (out += std::to_string(components[I]) + (I == N-1 ? "" : ", "), 0)...
                };
                out += " }";
                return out;
            }
    };
    
    using Monochrome = UniformColor<Mono, uint16_t, uint8_t>;
    using RGBAColor = UniformColor<>;
    using RGBColor = UniformColor<RGB>;
    using HDRColor = UniformColor<RGBA, int64_t, int16_t>;
    
    template <typename Color = RGBAColor, std::size_t Nelems = 256>
    struct Palette {
        static constexpr std::size_t N = Nelems;
        using color_t = Color;
        using component_t = typename Color::component_t;
        using channel_t = typename Color::channel_t;
        using composite_t = typename Color::composite_t;
        using channel_list_t = std::initializer_list<channel_t>;
        using channel_listlist_t = std::initializer_list<channel_list_t>;
        using composite_list_t = std::initializer_list<composite_t>;
        using composite_listlist_t = std::initializer_list<composite_list_t>;
        
        std::set<Color> items;
        
        constexpr Palette(const Palette& other)
            :items(other.items)
            {}
        constexpr Palette(Palette&& other) {
            items.swap(other.items);
        }
        
        explicit constexpr Palette(composite_list_t initlist)     { add_impl(initlist); }
        explicit constexpr Palette(channel_listlist_t initlist)   { add_impl(initlist); }
        Palette &operator=(const Palette& other)        { items = other.items;                      return *this; }
        Palette &operator=(Palette&& other)             { items.clear(); items.swap(other.items);   return *this; }
        Palette &operator=(composite_list_t initlist)   { items.clear(); add_impl(initlist);        return *this; }
        Palette &operator=(channel_listlist_t initlist) { items.clear(); add_impl(initlist);        return *this; }
        
        constexpr bool add(composite_t composite)                 { return items.emplace(composite).second; }
        constexpr bool add(channel_list_t channel_list)           { return items.emplace(channel_list).second; }
        constexpr bool bulk_add(composite_list_t composite_list)  { return add_impl(composite_list); }
        constexpr bool bulk_add(channel_listlist_t channel_list)  { return add_impl(channel_list); }
        
        constexpr std::size_t max_size() const { return N; }
        const std::size_t size() const { return items.size(); }
        const bool contains(composite_t composite) const {
            return static_cast<bool>(items.count(composite));
        }
        
        Color &operator[](composite_t composite) {
            auto search = items.find(composite);
            if (search != items.end()) { return Color(*search); }
            return Color(0); /// WE NEED TO DO BETTER THAN THIS
        }
        
        constexpr bool operator<(const Palette& rhs) noexcept { return items < rhs.items; }
        constexpr bool operator>(const Palette& rhs) noexcept { return items > rhs.items; }
        constexpr bool operator<=(const Palette& rhs) noexcept { return items <= rhs.items; }
        constexpr bool operator>=(const Palette& rhs) noexcept { return items >= rhs.items; }
        constexpr bool operator==(const Palette& rhs) noexcept { return items == rhs.items; }
        constexpr bool operator!=(const Palette& rhs) noexcept { return items != rhs.items; }
        
        private:
            template <typename List> inline
            bool add_impl(List list) {
                int idx;
                const int siz = idx = size();
                auto seterator = items.begin();
                for (auto it = list.begin();
                     it != list.end() && idx < N;
                     ++it) { seterator = items.emplace_hint(seterator, *it); ++idx; }
                return siz < size();
            }
        
    };
    
}

#ifdef ALIGN_AS
#undef ALIGN_AS
#endif

#endif /// LIBIMREAD_PALETTE_HH_