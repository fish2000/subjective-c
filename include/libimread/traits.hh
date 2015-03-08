
#include <type_traits>
#include <libimread/libimread.hpp>

//#define TEST_ARGS byte_source*, ImageFactory*, const options_map&
#define TEST_ARGS T

namespace im {
    
    template <typename>
    struct sfinae_true : std::true_type{};

    namespace detail {
        template <typename T, typename ...Args>
        static auto test_read(int) -> typename T::can_read;
        template <typename, typename ...Args>
        static auto test_read(long) -> std::false_type;
        
        template <typename T, typename ...Args>
        static auto test_write(int) -> typename T::can_write;
        template <typename, typename ...Args>
        static auto test_write(long) -> std::false_type;
        
        template <typename T, typename ...Args>
        static auto test_read_multi(int) -> typename T::can_read_multi;
        template <typename, typename ...Args>
        static auto test_read_multi(long) -> std::false_type;
        
        template <typename T, typename ...Args>
        static auto test_write_metadata(int) -> typename T::can_write_metadata;
        template <typename, typename ...Args>
        static auto test_write_metadata(long) -> std::false_type;
    }
    
    template <typename T>
    struct has_read : decltype(detail::test_read<T, TEST_ARGS>(0)) {
        template <typename X = std::enable_if<decltype(detail::test_read<T, TEST_ARGS>(0))::value>>
        static constexpr bool value() { return true; }
        static constexpr bool value() { return detail::test_read<T, TEST_ARGS>(0); }
    };
    
    template <typename T>
    struct has_write : decltype(detail::test_write<T, TEST_ARGS>(0)){
        template <typename X = std::enable_if<decltype(detail::test_write<T, TEST_ARGS>(0))::value>>
        static constexpr bool value() { return true; }
        static constexpr bool value() { return detail::test_write<T, TEST_ARGS>(0); }
    };
    
    template <typename T>
    struct has_read_multi : decltype(detail::test_read_multi<T, TEST_ARGS>(0)){
        template <typename X = std::enable_if<decltype(detail::test_read_multi<T, TEST_ARGS>(0))::value>>
        static constexpr bool value() { return true; }
        static constexpr bool value() { return detail::test_read_multi<T, TEST_ARGS>(0); }
    };
    
    template <typename T>
    struct has_write_metadata : decltype(detail::test_write_metadata<T, TEST_ARGS>(0)){
        template <typename X = std::enable_if<decltype(detail::test_write_metadata<T, TEST_ARGS>(0))::value>>
        static constexpr bool value() { return true; }
        static constexpr bool value() { return detail::test_write_metadata<T, TEST_ARGS>(0); }
    };

} /// namespace im

#undef TEST_ARGS
