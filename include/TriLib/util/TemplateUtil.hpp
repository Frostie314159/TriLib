#ifndef TEMPLATE_UTIL_HPP_
#define TEMPLATE_UTIL_HPP_

#include <type_traits>
//This header is necessary to make the code compatible with c++11(SFINAE) and c++20(constraints and concepts)

#if __cplusplus == 1
#error "Compiler is outdated or providing wrong information"
#elif __cplusplus == 201103L || __cplusplus == 201402L
#define ARITHMETIC_TEMPLATE(x) template<typename x, typename = std::enable_if<std::is_arithmetic<x>::value || std::integral_constant<bool, !std::is_const<x>::value>::value>>
#define SSE_REGISTER_TEMPLATE(x) template<typename x, typename = std::enable_if<std::is_same<tl::detail::__m64_wrapper, x>::value || std::is_same<tl::detail::__m128_wrapper, x>::value || std::is_same<tl::detail::__m128d_wrapper, x>::value || std::is_same<tl::detail::__m128i_wrapper, x>::value>>
namespace tl::templates{
    template<bool A, bool B>
    using OR = std::integral_constant<bool, !(A || B)>;
}
#elif __cplusplus == 201703L
#define ARITHMETIC_TEMPLATE(x) template<typename x, typename = std::enable_if_t<std::is_arithmetic_v<x>>>
#define SSE_REGISTER_TEMPLATE(x) template<typename x, typename = std::enable_if_t<std::is_same_v<tl::detail::__m64_wrapper, x> || std::is_same_v<tl::detail::__m128_wrapper, x> || std::is_same_v<tl::detail::__m128d_wrapper, x> || std::is_same_v<tl::detail::__m128i_wrapper, x>>>
namespace tl::templates{
    template<typename A, typename B>
    using OR = std::bool_constant<std::disjunction_v<A, B>>;
}
#elif __cplusplus == 202002L
#define ARITHMETIC_TEMPLATE(x) template<typename x> requires std::is_arithmetic_v<x>
#define SSE_REGISTER_TEMPLATE(x) template<typename x> requires std::is_same_v<tl::detail::__m64_wrapper, x> || std::is_same_v<tl::detail::__m128_wrapper, x> || std::is_same_v<tl::detail::__m128d_wrapper, x> || std::is_same_v<tl::detail::__m128i_wrapper, x>
namespace tl::templates{
    template<typename A, typename B>
    using OR = std::bool_constant<std::disjunction_v<A, B>>;
}
#endif

#endif