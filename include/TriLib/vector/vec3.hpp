#ifndef VEC_3_HPP_
#define VEC_3_HPP_
#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

#include "TriLib/vector/Base_Vector.hpp"
#include "TriLib/util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"

#include "Base_Vector.hpp"
BEGIN_NAMESPACE_INTERNAL //This is done because nested namespaces have only been available since c++17
    using std::string_literals::operator""s;

    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct _vec3: public vec{
        _ArithmeticType x, y, z;
        //Constructor and Destructor section
        _vec3(_ArithmeticType t_x, _ArithmeticType t_y, _ArithmeticType t_z): x(t_x), y(t_y), z(t_z){}
        _vec3(_ArithmeticType t_x): x(t_x), y(t_x), z(t_x){}
        _vec3(const _vec3&) = default;
        _vec3(){}
        ~_vec3(){}
        

        //Operator overloading is SSE/AVX optimized
#ifdef __AVX__
        static inline tl::detail::__m128i_wrapper loadToSSERegister(_vec3<int32_t> t_vector){
            return tl::detail::__m128i_wrapper{_mm_setr_epi32(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline tl::detail::__m128_wrapper loadToSSERegister(_vec3<float_t> t_vector){
            return tl::detail::__m128_wrapper{_mm_setr_ps(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline tl::detail::__m256i_wrapper loadToAVXRegister(_vec3<int64_t> t_vector){
            return tl::detail::__m256i_wrapper{_mm256_setr_epi64x(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline tl::detail::__m256d_wrapper loadToSSERegister(_vec3<double_t> t_vector){
            return tl::detail::__m256d_wrapper{_mm256_setr_pd(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline _vec3<int32_t> loadToVec3(__m128i t_sseRegister){
            return _vec3<int32_t>(t_sseRegister[0], t_sseRegister[1], t_sseRegister[2]);
        }
        static inline _vec3<float_t> loadToVec3(__m128 t_sseRegister){
            return _vec3<float_t>(t_sseRegister[0], t_sseRegister[1], t_sseRegister[2]);
        }
        static inline _vec3<int64_t> loadToVec3(__m256i t_avxRegister){
            return _vec3<int64_t>(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2]);
        }
        static inline _vec3<double_t> loadToVec3(__m256d t_avxRegister){
            return _vec3<double_t>(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2]);
        }
        
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator+(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_add_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm256_add_epi64(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm256_add_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_add_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_add_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm256_add_epi64(loadToAVXRegister(*this), loadToAVXRegister(static_cast<_vec3<int64_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_add_epi64(loadToAVXRegister(static_cast<_vec3<int64_t>>(*this)), loadToAVXRegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator-(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_sub_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm256_sub_epi64(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm256_sub_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_sub_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_sub_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm256_sub_epi64(loadToAVXRegister(*this), loadToAVXRegister(static_cast<_vec3<int64_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_sub_epi64(loadToAVXRegister(static_cast<_vec3<int64_t>>(*this)), loadToAVXRegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator*(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_mul_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm256_mul_epi64(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm256_mul_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_mul_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_mul_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm256_mul_epi64(loadToAVXRegister(*this), loadToAVXRegister(static_cast<_vec3<int64_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_mul_epi64(loadToAVXRegister(static_cast<_vec3<int64_t>>(*this)), loadToAVXRegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator/(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_div_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm256_div_epi64(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm256_div_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_div_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_div_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm256_div_epi64(loadToAVXRegister(*this), loadToAVXRegister(static_cast<_vec3<int64_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm256_div_epi64(loadToAVXRegister(static_cast<_vec3<int64_t>>(*this)), loadToAVXRegister(t_other)));
                    }
                }
            }
        }
#elif defined(__SSE2__)
        static inline tl::detail::__m128i_wrapper loadToSSERegister(_vec3<int32_t> t_vector){
            return tl::detail::__m128i_wrapper{_mm_setr_epi32(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline tl::detail::__m128_wrapper loadToSSERegister(_vec3<float_t> t_vector){
            return tl::detail::__m128_wrapper{_mm_setr_ps(t_vector.x, t_vector.y, t_vector.z, 0)};
        }
        static inline tl::detail::__m128i_wrapper loadToSSERegister(_vec3<int64_t> t_vector){
            return tl::detail::__m128i_wrapper{_mm_setr_epi64(reinterpret_cast<__m64>(t_vector.x), reinterpret_cast<__m64>(t_vector.y))};
        }
        static inline tl::detail::__m128d_wrapper loadToSSERegister(_vec3<double_t> t_vector){
            return tl::detail::__m128d_wrapper{_mm_setr_pd(t_vector.x, t_vector.y)};
        }
        static inline _vec3<int32_t> loadToVec3(__m128i t_sseRegister){
            return _vec3<int32_t>(t_sseRegister[0], t_sseRegister[1], t_sseRegister[2]);
        }
        static inline _vec3<int64_t> loadToVec3(__m128i t_sseRegister, int64_t t_z){
            return _vec3<int64_t>(t_sseRegister[0], t_sseRegister[1], t_z);
        }
        static inline _vec3<float_t> loadToVec3(__m128 t_sseRegister){
            return _vec3<float_t>(t_sseRegister[0], t_sseRegister[1], t_sseRegister[2]);
        }
        static inline _vec3<double_t> loadToVec3(__m128d t_sseRegister, double_t t_z){
            return _vec3<double_t>(t_sseRegister[0], t_sseRegister[1], t_z);
        }

        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator+(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_add_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm_add_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z + t_other.z);
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z + t_other.z);
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))), this->z + t_other.z);
                    }else{
                        return loadToVec3(_mm_add_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)), this->z + t_other.z);
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_add_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm_add_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<int64_t>>(t_other))), this->z + t_other.z);
                    }else{
                        return loadToVec3(_mm_add_epi64(loadToSSERegister(static_cast<_vec3<int64_t>>(*this)), loadToSSERegister(t_other)), this->z + t_other.z);
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator-(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_sub_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm_sub_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z - t_other.z);
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z - t_other.z);
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))), this->z - t_other.z);
                    }else{
                        return loadToVec3(_mm_sub_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)), this->z - t_other.z);
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_sub_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm_sub_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<int64_t>>(t_other))), this->z - t_other.z);
                    }else{
                        return loadToVec3(_mm_sub_epi64(loadToSSERegister(static_cast<_vec3<int64_t>>(*this)), loadToSSERegister(t_other)), this->z - t_other.z);
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator*(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_mul_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm_mul_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z * t_other.z);
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z * t_other.z);
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))), this->z * t_other.z);
                    }else{
                        return loadToVec3(_mm_mul_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)), this->z * t_other.z);
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_mul_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm_mul_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<int64_t>>(t_other))), this->z * t_other.z);
                    }else{
                        return loadToVec3(_mm_mul_epi64(loadToSSERegister(static_cast<_vec3<int64_t>>(*this)), loadToSSERegister(t_other)), this->z * t_other.z);
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec3 operator/(_vec3<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec3(_mm_div_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec3(_mm_div_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z / t_other.z);
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec3(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec3(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z / t_other.z);
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec3(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<double_t>>(t_other))), this->z / t_other.z);
                    }else{
                        return loadToVec3(_mm_div_pd(loadToSSERegister(static_cast<_vec3<double_t>>(*this)), loadToSSERegister(t_other)), this->z / t_other.z);
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec3(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<float_t>>(t_other))));
                    }else{
                        return loadToVec3(_mm_div_ps(loadToSSERegister(static_cast<_vec3<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec3(_mm_div_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec3<int64_t>>(t_other))), this->z / t_other.z);
                    }else{
                        return loadToVec3(_mm_div_epi64(loadToSSERegister(static_cast<_vec3<int64_t>>(*this)), loadToSSERegister(t_other)), this->z / t_other.z);
                    }
                }
            }
        }
#else
        _vec3 operator+(_vec3 t_other){
            return _vec3(this->x + t_other.x, this->y + t_other.y, this->z + t_other.z);
        }
        _vec3 operator-(_vec3 t_other){
            return _vec3(this->x - t_other.x, this->y - t_other.y, this->z - t_other.z);
        }
        _vec3 operator*(_vec3 t_other){
            return _vec3(this->x * t_other.x, this->y * t_other.y, this->z * t_other.z);
        }
        _vec3 operator/(_vec3 t_other){
            return _vec3(this->x / t_other.x, this->y / t_other.y, this->z / t_other.z);
        }
#endif
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator==(const _vec3<_VectorType>& t_other){
            return this->x == t_other.x && this->y == t_other.y && this->z == t_other.z;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator!=(const _vec3<_VectorType>& t_other){
            return this->x != t_other.x && this->y != t_other.y && this->z != t_other.z;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<(const _vec3<_VectorType>& t_other){
            return this->x < t_other.x && this->y < t_other.y && this->z < t_other.z;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>(const _vec3<_VectorType>& t_other){
            return this->x > t_other.x && this->y > t_other.y && this->z > t_other.z;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<=(const _vec3<_VectorType>& t_other){
            return this->x <= t_other.x && this->y <= t_other.y && this->z <= t_other.z;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>=(const _vec3<_VectorType>& t_other){
            return this->x >= t_other.x && this->y >= t_other.y && this->z >= t_other.z;
        }
        _vec3 operator=(const _vec3& t_other){
            if(this != &t_other){
                this->x = t_other.x;
                this->y = t_other.y;
                this->z = t_other.z;
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return _vec3<_ArithmeticType>(0);//This will never be called because an exception was previously thrown.
                }
            }
            return *this;
        }
        
        //Casting section
        ARITHMETIC_TEMPLATE(_CastVectorType)
        operator _vec3<_CastVectorType>(){
            return _vec3<_CastVectorType>(static_cast<_CastVectorType>(this->x), static_cast<_CastVectorType>(this->y), static_cast<_CastVectorType>(this->z));
        }

        //Advanced math functions
        constexpr _ArithmeticType accumulate(){
            return this->x + this->y + this->z;
        }
        //The functions getLength() and dot(_vec3<T>) are almost completely unchanged due to operator overloading
        constexpr _ArithmeticType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float_t dot(_vec3<_VectorType> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr _vec3<_ArithmeticType> normalise(){
            _ArithmeticType m_magnitude = this->getLength();
            return (*this) / _vec3{m_magnitude};
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 3;
        }
        inline std::string toString(){
            return std::string("_vec3("s + std::to_string(this->x) + ","s + std::to_string(this->y) + ","s + std::to_string(this->z) + ")"s);
        }
    };
END_NAMESPACE_INTERNAL
#endif