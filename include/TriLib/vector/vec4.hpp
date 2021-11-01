#ifndef VEC_4_HPP_
#define VEC_4_HPP_

#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

#include "TriLib/vector/Base_Vector.hpp"
#include "TriLib/util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"

BEGIN_NAMESPACE_INTERNAL //This is done because nested namespaces have only been available since c++17
    using std::string_literals::operator""s;

    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct _vec4: public vec{
        _ArithmeticType x, y, z, w;
        
        _vec4(_ArithmeticType t_x, _ArithmeticType t_y, _ArithmeticType t_z, _ArithmeticType t_w): x(t_x), y(t_y), z(t_z), w(t_w){}
        _vec4(_ArithmeticType t_x): x(t_x), y(t_x), z(t_x), w(t_x){}
        _vec4(const _vec4&) = default;
        _vec4(){}
        ~_vec4(){}
        
#ifdef __AVX__
        static inline __m256d loadToAVXRegister(_vec4<double> t_vector){
            return _mm256_setr_pd(t_vector.x, t_vector.y, t_vector.z, t_vector.w);
        }
        static inline __m256d loadToAVXRegister(_vec4<float> t_vector){
            return _mm256_setr_pd(t_vector.x, t_vector.y, t_vector.z, t_vector.w);
        }
        static inline _vec4 loadToVec4(__m256d t_avxRegister){
            return _vec4(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2], t_avxRegister[3]);
        }

        _vec4 operator+(_vec4 t_other){
            return loadToVec4(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec4 operator-(_vec4 t_other){
            return loadToVec4(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec4 operator*(_vec4 t_other){
            return loadToVec4(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec4 operator/(_vec4 t_other){
            return loadToVec4(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        constexpr _ArithmeticType accumulate(){
            //manual SSE optimization
            __m128d m_temp = _mm_add_pd(_mm_setr_pd(this->x, this->y), _mm_setr_pd(this->z, this->w));
            return m_temp[0] + m_temp[1];
        }
#elif defined(__SSE2__)
        static inline __m128d loadToFirstSSERegister(_vec4<double> t_vector){
            return _mm_setr_pd(t_vector.x, t_vector.y);
        }
        static inline __m128d loadToFirstSSERegister(_vec4<float> t_vector){
            return _mm_setr_pd(t_vector.x, t_vector.y);
        }
        static inline __m128d loadToSecondSSERegister(_vec4<double> t_vector){
            return _mm_setr_pd(t_vector.z, t_vector.w);
        }
        static inline __m128d loadToSecondSSERegister(_vec4<float> t_vector){
            return _mm_setr_pd(t_vector.z, t_vector.w);
        }
        static inline _vec4 loadToVec4(__m128d t_firstSSERegister, __m128d t_secondSSERegister){
            return _vec4(t_firstSSERegister[0], t_firstSSERegister[1], t_secondSSERegister[0], t_secondSSERegister[1]);
        }

        _vec4 operator+(_vec4 t_other){
            return loadToVec4(_mm_add_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_add_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        _vec4 operator-(_vec4 t_other){
            return loadToVec4(_mm_sub_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_sub_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        _vec4 operator*(_vec4 t_other){
            return loadToVec4(_mm_mul_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_mul_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        _vec4 operator/(_vec4 t_other){
            return loadToVec4(_mm_div_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_div_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        constexpr _ArithmeticType accumulate(){
            //manual SSE optimization
            __m128d m_temp = _mm_add_pd(loadToFirstSSERegister(*this),loadToSecondSSERegister(*this));
            return m_temp[0] + m_temp[1];
        }
#else

        _vec4 operator+(_vec4 t_other){
            return _vec4(this->x + t_other.x, this->y + t_other.y, this->z + t_other.z, this->w + t_other.w);
        }
        _vec4 operator-(_vec4 t_other){
            return _vec4(this->x - t_other.x, this->y - t_other.y, this->z - t_other.z, this->w - t_other.w);
        }
        _vec4 operator*(_vec4 t_other){
            return _vec4(this->x * t_other.x, this->y * t_other.y, this->z * t_other.z, this->w * t_other.w);
        }
        _vec4 operator/(_vec4 t_other){
            return _vec4(this->x / t_other.x, this->y / t_other.y, this->z / t_other.z, this->w / t_other.w);
        }
        constexpr _ArithmeticType accumulate(){
            return this->x + this->y + this->z + this->w;
        }
#endif
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator==(const _vec4<_VectorType>& t_other){
            return this->x == t_other.x && this->y == t_other.y && this->z == t_other.z && this->w == t_other.w;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator!=(const _vec4<_VectorType>& t_other){
            return this->x != t_other.x && this->y != t_other.y && this->z != t_other.z && this->w != t_other.w;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<(const _vec4<_VectorType>& t_other){
            return this->x < t_other.x && this->y < t_other.y && this->z < t_other.z && this->w < t_other.w;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>(const _vec4<_VectorType>& t_other){
            return this->x > t_other.x && this->y > t_other.y && this->z > t_other.z && this->w > t_other.w;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<=(const _vec4<_VectorType>& t_other){
            return this->x <= t_other.x && this->y <= t_other.y && this->z <= t_other.z && this->w <= t_other.w;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>=(const _vec4<_VectorType>& t_other){
            return this->x >= t_other.x && this->y >= t_other.y && this->z >= t_other.z && this->w >= t_other.w;
        }
        _vec4 operator=(const _vec4& t_other){
            if(this != &t_other){
                this->x = t_other.x;
                this->y = t_other.y;
                this->z = t_other.z;
                this->w = t_other.w;
                
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return _vec4<_ArithmeticType>(0);//This will never be called because an exception was previously thrown.
                }
            }
            return *this;
        }
        
        //Casting section
        ARITHMETIC_TEMPLATE(_CastVectorType)
        operator _vec4<_CastVectorType>(){
            return _vec4<_CastVectorType>(static_cast<_CastVectorType>(this->x), static_cast<_CastVectorType>(this->y), static_cast<_CastVectorType>(this->z), static_cast<_CastVectorType>(this->w));
        }

        //Advanced Math Section
        constexpr _ArithmeticType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float dot(_vec4<_VectorType> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr _vec4<_ArithmeticType> normalise(){
            _ArithmeticType m_magnitude = this->getLength();
            return (*this) / _vec4<_ArithmeticType>{m_magnitude};
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 4;
        }
        inline std::string toString(){
            return std::string("_vec4("s + std::to_string(this->x) + ","s + std::to_string(this->y) + ","s + std::to_string(this->z) + ","s + std::to_string(this->w) + ")"s);
        }
    };
END_NAMESPACE_INTERNAL
#endif