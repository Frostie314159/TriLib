#ifndef VEC_2_HPP_
#define VEC_2_HPP_

#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

#include "TriLib/util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"
#include "TriLib/vector/Base_Vector.hpp"

namespace tl::internal{
    using std::string_literals::operator""s;
    //Template means: _ArithmeticType must be of arithmetic type
    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct _vec2: public vec{
        //Data Section
        _ArithmeticType x, y;
        //Constructor and Destructor Section
        _vec2(_ArithmeticType t_x, _ArithmeticType t_y): x(t_x), y(t_y){}
        _vec2(_ArithmeticType t_x): x(t_x), y(t_x){}
        _vec2(const _vec2&) = default;
        _vec2(){}
        ~_vec2(){}

        //Operator overloading is SSE optimized
#ifdef __SSE2__
        ARITHMETIC_TEMPLATE(_VectorType)
        static inline std::conditional<std::is_same<int, _VectorType>::value, tl::detail::__m64_wrapper, std::conditional<std::is_same<float, _VectorType>::value, tl::detail::__m128_wrapper, std::conditional<std::is_same<double, _VectorType>::value, tl::detail::__m128d_wrapper, tl::detail::__m128i_wrapper>>> loadToSSERegister(_vec2<_VectorType> t_vector){
            if constexpr(std::is_same<float, _VectorType>::value)
                return _mm_setr_ps(t_vector.x, t_vector.y, 0, 0);
            else if constexpr(std::is_same<double, _VectorType>::value)
                return _mm_setr_pd(t_vector.x, t_vector.y);
            else if constexpr(std::is_same<int32_t, _VectorType>::value || std::is_same<uint32_t, _VectorType>::value)
                return _mm_setr_pi32(t_vector.x, t_vector.y);
            else
                return _mm_setr_epi64(static_cast<__m64>(t_vector.x), static_cast<__m64>(t_vector.y));

        }
        SSE_REGISTER_TEMPLATE(_SSERegisterPrimitiveType)
        static inline _vec2<tl::detail::PrimitveRegisterType<_SSERegisterPrimitiveType>> loadTo_vec2(_SSERegisterPrimitiveType t_sseRegister){
            return _vec2<tl::detail::PrimitveRegisterType<_SSERegisterPrimitiveType>>(t_sseRegister[0], t_sseRegister[1]);
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        _vec2 operator+(_vec2<_VectorType> t_other){
            return loadTo_vec2(_mm_sub_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
        _vec2 operator-(_vec2 t_other){
            return loadTo_vec2(_mm_sub_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
        _vec2 operator*(_vec2 t_other){
            return loadTo_vec2(_mm_mul_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
        _vec2 operator/(_vec2 t_other){
            return loadTo_vec2(_mm_div_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
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
        _vec2& operator=(const _vec2 t_other){
            if(this != &t_other){
                this->x = t_other.x;
                this->y = t_other.y;
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return vec2<_ArithmeticType>(0);
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
        constexpr _vec2<_ArithmeticType> normalize(){
            //Updated normalization function.
            //If the Host System supports SSE this will be optimized with said Instruction Set Extension.
            _ArithmeticType m_magnitude = this->getLength(); 
            return *this / _vec2<_ArithmeticType>{m_magnitude};
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float dot(_vec2<_VectorType> t_other){
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
}
#endif