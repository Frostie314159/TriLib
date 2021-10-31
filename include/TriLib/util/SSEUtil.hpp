#ifndef SSE_UTIL_
#define SSE_UTIL_

#include <type_traits>
#ifdef __AVX__
#include <immintrin.h>
#else
#include <emmintrin.h>
#endif

#include "TriLib/util/NamespaceUtil.hpp"

BEGIN_NAMESPACE_DETAIL
    typedef struct{
        __m64 m_m64;
        operator __m64(){
            return m_m64;
        }
        int32_t operator[](size_t t_index){
            return m_m64[t_index];
        }
        
    }__m64_wrapper;
    typedef struct{
        __m128 m_m128;
        operator __m128(){
            return m_m128;
        }
        float_t operator[](size_t t_index){
            return m_m128[t_index];
        }
    }__m128_wrapper;
    typedef struct{
        __m128d m_m128d;
        operator __m128d(){
            return m_m128d;
        }
        double_t operator[](size_t t_index){
            return m_m128d[t_index];
        }
    }__m128d_wrapper;
    typedef struct{
        __m128i m_m128i;
        operator __m128i(){
            return m_m128i;
        }
        int64_t operator[](size_t t_index){
            return m_m128i[t_index];
        }
    }__m128i_wrapper;
#ifdef __AVX__
    typedef struct{
        __m256d m_m256d;
        operator __m256d(){
            return m_m256d;
        }
        double_t operator[](size_t t_index){
            return m_m256d[t_index];
        }
    }__m256d_wrapper;
    typedef struct{
        __m256i m_m256i;
        operator __m256i(){
            return m_m256i;
        }
        int64_t operator[](size_t t_index){
            return m_m256i[t_index];
        }
    }__m256i_wrapper;
#endif
    template<typename T>
    using PrimitveRegisterType = std::conditional<std::is_same<__m64_wrapper, T>::value, int, std::conditional<std::is_same<__m128_wrapper, T>::value, float, std::conditional<std::is_same<__m128d_wrapper, T>::value, double, long>>>;
    template<typename T>
    using RegisterType = std::conditional<std::is_same<int, T>::value, __m64_wrapper, std::conditional<std::is_same<float, T>::value, __m128_wrapper, std::conditional<std::is_same<double, T>::value, __m128d_wrapper, __m128i_wrapper>>>;
END_NAMESPACE_DETAIL
#endif