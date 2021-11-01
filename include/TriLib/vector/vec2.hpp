#ifndef VEC_2_HPP_
#define VEC_2_HPP_

#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

#include "TriLib/util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"
#include "TriLib/vector/Base_Vector.hpp"

BEGIN_NAMESPACE_INTERNAL //This is done because nested namespaces have only been available since c++17
    using std::string_literals::operator""s;
    //Template means: _ArithmeticType must be of arithmetic type
    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct _vec2: public vec{
        //Data Section
        _ArithmeticType x, y;
        //Constructor and Destructor Section
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2(_vec2<_VectorType>& t_vector): x(t_vector.x), y(t_vector.y){}
        _vec2(_ArithmeticType t_x, _ArithmeticType t_y): x(t_x), y(t_y){}
        _vec2(_ArithmeticType t_x): x(t_x), y(t_x){}
        _vec2(const _vec2&) = default;
        _vec2(){}
        ~_vec2(){}

        //Operator overloading is SSE optimized
#ifdef __SSE2__

        static inline tl::detail::__m64_wrapper loadToSSERegister(_vec2<int32_t> t_vector){
            return tl::detail::__m64_wrapper{_mm_setr_pi32(t_vector.x, t_vector.y)};
        }
        static inline tl::detail::__m128_wrapper loadToSSERegister(_vec2<float_t> t_vector){
            return tl::detail::__m128_wrapper{_mm_setr_ps(t_vector.x, t_vector.y, 0, 0)};
        }
        static inline tl::detail::__m128i_wrapper loadToSSERegister(_vec2<int64_t> t_vector){
            return tl::detail::__m128i_wrapper{_mm_setr_epi64(reinterpret_cast<__m64>(t_vector.x), reinterpret_cast<__m64>(t_vector.y))};
        }
        static inline tl::detail::__m128d_wrapper loadToSSERegister(_vec2<double_t> t_vector){
            return tl::detail::__m128d_wrapper{_mm_setr_pd(t_vector.x, t_vector.y)};
        }
        static inline _vec2<int32_t> loadToVec2(__m64 t_sseRegister){
            return _vec2<int32_t>(t_sseRegister[0], t_sseRegister[1]);
        }
        static inline _vec2<float_t> loadToVec2(__m128 t_sseRegister){
            return _vec2<float_t>(t_sseRegister[0], t_sseRegister[1]);
        }
        static inline _vec2<int64_t> loadToVec2(__m128i t_sseRegister){
            return _vec2<int64_t>(t_sseRegister[0], t_sseRegister[1]);
        }
        static inline _vec2<double_t> loadToVec2(__m128d t_sseRegister){
            return _vec2<double_t>(t_sseRegister[0], t_sseRegister[1]);
        }

        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2 operator+(_vec2<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec2(_mm_add_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec2(_mm_add_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec2(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec2(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec2(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<double_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_add_pd(loadToSSERegister(static_cast<_vec2<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec2(_mm_add_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<float_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_add_ps(loadToSSERegister(static_cast<_vec2<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec2(_mm_add_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<int64_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_add_epi64(loadToSSERegister(static_cast<_vec2<int64_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2 operator-(_vec2<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return loadToVec2(_mm_sub_pi32(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec2(_mm_sub_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec2(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec2(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec2(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<double_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_sub_pd(loadToSSERegister(static_cast<_vec2<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec2(_mm_sub_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<float_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_sub_ps(loadToSSERegister(static_cast<_vec2<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec2(_mm_sub_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<int64_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_sub_epi64(loadToSSERegister(static_cast<_vec2<int64_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2 operator*(_vec2<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return _vec2<int32_t>(this->x * t_other.x, this->y * t_other.y);
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec2(_mm_mul_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec2(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec2(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec2(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<double_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_mul_pd(loadToSSERegister(static_cast<_vec2<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec2(_mm_mul_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<float_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_mul_ps(loadToSSERegister(static_cast<_vec2<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec2(_mm_mul_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<int64_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_mul_epi64(loadToSSERegister(static_cast<_vec2<int64_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }
            }
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2 operator/(_vec2<_VectorType> t_other){
            if constexpr(std::is_same<_ArithmeticType, _VectorType>::value){
                if constexpr(std::is_same<_ArithmeticType, int32_t>::value){
                    return _vec2<int32_t>(this->x / t_other.x, this->y / t_other.y);
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                    return loadToVec2(_mm_div_epi64(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                    return loadToVec2(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }else if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                    return loadToVec2(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(t_other)));
                }
            }else{
                if constexpr(std::is_same<_ArithmeticType, double_t>::value || std::is_same<_VectorType, double_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, double_t>::value){
                        return loadToVec2(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<double_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_div_pd(loadToSSERegister(static_cast<_vec2<double_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, float_t>::value || std::is_same<_VectorType, float_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, float_t>::value){
                        return loadToVec2(_mm_div_ps(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<float_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_div_ps(loadToSSERegister(static_cast<_vec2<float_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }else if constexpr(std::is_same<_ArithmeticType, int64_t>::value || std::is_same<_VectorType, int64_t>::value){
                    if constexpr(std::is_same<_ArithmeticType, int64_t>::value){
                        return loadToVec2(_mm_div_epi64(loadToSSERegister(*this), loadToSSERegister(static_cast<_vec2<int64_t>>(t_other))));
                    }else{
                        return loadToVec2(_mm_div_epi64(loadToSSERegister(static_cast<_vec2<int64_t>>(*this)), loadToSSERegister(t_other)));
                    }
                }
            }
        }
#else
        _vec2 operator+(_vec2 t_other){
            return _vec2(this->x + t_other.x, this->y + t_other.y);
        }
        _vec2 operator-(_vec2 t_other){
            return _vec2(this->x - t_other.x, this->y - t_other.y);
        }
        _vec2 operator*(_vec2 t_other){
            return _vec2(this->x * t_other.x, this->y * t_other.y);
        }
        _vec2 operator/(_vec2 t_other){
            return _vec2(this->x / t_other.x, this->y / t_other.y);
        }
#endif
        _vec2 operator=(const _vec2 t_other){
            if(this != &t_other){
                this->x = t_other.x;
                this->y = t_other.y;
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return _vec2<_ArithmeticType>(0);
                }
            }
            return *this;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator==(const _vec2<_VectorType>& t_other){
            return this->x == t_other.x && this->y == t_other.y;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator!=(const _vec2<_VectorType>& t_other){
            return this->x != t_other.x && this->y != t_other.y;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<(const _vec2<_VectorType>& t_other){
            return this->x < t_other.x && this->y < t_other.y;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>(const _vec2<_VectorType>& t_other){
            return this->x > t_other.x && this->y > t_other.y;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<=(const _vec2<_VectorType>& t_other){
            return this->x <= t_other.x && this->y <= t_other.y;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>=(const _vec2<_VectorType>& t_other){
            return this->x >= t_other.x && this->y >= t_other.y;
        }
        
        //Casting section
        ARITHMETIC_TEMPLATE(_CastVectorType)
        operator _vec2<_CastVectorType>(){
            return _vec2<_CastVectorType>(static_cast<_CastVectorType>(this->x), static_cast<_CastVectorType>(this->y));
        }

        constexpr _ArithmeticType accumulate(){
            //Adding X and Y isn't obsolete here due to the Vector being multidimensional.
            return this->x + this->y;
        }
        constexpr _ArithmeticType getLength(){
            //Fast Way can't be used here so we need to use sqrt(x²+y²).
            //Depending on the host system this will be SSE optimized.
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        constexpr _vec2<_ArithmeticType> normalise(){
            //Updated normalization function.
            //If the Host System supports SSE this will be optimized with said Instruction Set Extension.
            _ArithmeticType m_magnitude = this->getLength(); 
            return *this / _vec2<_ArithmeticType>{m_magnitude};
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float_t dot(_vec2<_VectorType> t_other){
            //Due to Operator overloading this function is actually always exactly the same.
            return (*this * t_other).accumulate();
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 2;
        }
        inline std::string toString(){
            return std::string("vec2("s + std::to_string(this->x) + ","s + std::to_string(this->y) + ")"s);
        }
    };
END_NAMESPACE_INTERNAL
#endif