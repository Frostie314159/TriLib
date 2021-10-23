#ifndef SSE_UTIL_
#define SSE_UTIL_

#include <type_traits>
#ifdef __AVX__
#include <immintrin.h>
#else
#include <emmintrin.h>
#endif

namespace tl::detail{ 
    typedef struct{
        __m64 m_m64;
    }__m64_wrapper;
    typedef struct{
        __m128 m_m128;
    }__m128_wrapper;
    typedef struct{
        __m128d m_m128d;
    }__m128d_wrapper;
    typedef struct{
        __m128i m_m128i;
    }__m128i_wrapper;
#ifdef __AVX__
    typedef struct{
        __m256d m_m256d;
    }__m256d_wrapper;
#endif
    template<typename T>
    using RegisterType = std::conditional<std::is_same<__m64_wrapper, T>::value, int, std::conditional<std::is_same<__m128_wrapper, T>::value, float, std::conditional<std::is_same<__m128d_wrapper, T>::value, double, long>>>;
}

#endif
