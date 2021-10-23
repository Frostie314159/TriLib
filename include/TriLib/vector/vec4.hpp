#ifndef VEC_4_HPP_
#define VEC_4_HPP_

#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

#include "TriLib/vector/Base_Vector.hpp"
#include "TriLib/util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"

namespace tl{
    using std::string_literals::operator""s;

    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct vec4: public vec{
    private:
        _ArithmeticType m_x, m_y, m_z, m_w;
    public:
        vec4(_ArithmeticType t_x, _ArithmeticType t_y, _ArithmeticType t_z, _ArithmeticType t_w): m_x(t_x), m_y(t_y), m_z(t_z), m_w(t_w){}
        vec4(_ArithmeticType t_x): m_x(t_x), m_y(t_x), m_z(t_x), m_w(t_x){}
        vec4(const vec4&) = default;
        vec4(){}
        ~vec4(){}
        
        inline _ArithmeticType getX(){
            return this->m_x;
        }
        inline _ArithmeticType getY(){
            return this->m_y;
        }
        inline _ArithmeticType getZ(){
            return this->m_z;
        }
        inline _ArithmeticType getW(){
            return this->m_w;
        }
#ifdef __AVX__
        static inline __m256d loadToAVXRegister(vec4<double> t_vector){
            return _mm256_setr_pd(t_vector.getX(), t_vector.getY(), t_vector.getZ(), t_vector.getW());
        }
        static inline __m256d loadToAVXRegister(vec4<float> t_vector){
            return _mm256_setr_pd(t_vector.getX(), t_vector.getY(), t_vector.getZ(), t_vector.getW());
        }
        static inline vec4 loadToVec4(__m256d t_avxRegister){
            return vec4(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2], t_avxRegister[3]);
        }

        vec4 operator+(vec4 t_other){
            return loadToVec4(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec4 operator-(vec4 t_other){
            return loadToVec4(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec4 operator*(vec4 t_other){
            return loadToVec4(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec4 operator/(vec4 t_other){
            return loadToVec4(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        constexpr _ArithmeticType accumulate(){
            //manual SSE optimization
            __m128d m_temp = _mm_add_pd(_mm_setr_pd(this->getX(), this->getY()), _mm_setr_pd(this->getZ(), this->getW()));
            return m_temp[0] + m_temp[1];
        }
#elif defined(__SSE2__)
        static inline __m128d loadToFirstSSERegister(vec4<double> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline __m128d loadToFirstSSERegister(vec4<float> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline __m128d loadToSecondSSERegister(vec4<double> t_vector){
            return _mm_setr_pd(t_vector.getZ(), t_vector.getW());
        }
        static inline __m128d loadToSecondSSERegister(vec4<float> t_vector){
            return _mm_setr_pd(t_vector.getZ(), t_vector.getW());
        }
        static inline vec4 loadToVec4(__m128d t_firstSSERegister, __m128d t_secondSSERegister){
            return vec4(t_firstSSERegister[0], t_firstSSERegister[1], t_secondSSERegister[0], t_secondSSERegister[1]);
        }

        vec4 operator+(vec4 t_other){
            return loadToVec4(_mm_add_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_add_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        vec4 operator-(vec4 t_other){
            return loadToVec4(_mm_sub_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_sub_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        vec4 operator*(vec4 t_other){
            return loadToVec4(_mm_mul_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_mul_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        vec4 operator/(vec4 t_other){
            return loadToVec4(_mm_div_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_div_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
        }
        constexpr _ArithmeticType accumulate(){
            //manual SSE optimization
            __m128d m_temp = _mm_add_pd(loadToFirstSSERegister(*this),loadToSecondSSERegister(*this));
            return m_temp[0] + m_temp[1];
        }
#else

        vec4 operator+(vec4 t_other){
            return vec4(this->getX() + t_other.getX(), this->getY() + t_other.getY(), this->getZ() + t_other.getZ(), this->getW() + t_other.getW());
        }
        vec4 operator-(vec4 t_other){
            return vec4(this->getX() - t_other.getX(), this->getY() - t_other.getY(), this->getZ() - t_other.getZ(), this->getW() - t_other.getW());
        }
        vec4 operator*(vec4 t_other){
            return vec4(this->getX() * t_other.getX(), this->getY() * t_other.getY(), this->getZ() * t_other.getZ(), this->getW() * t_other.getW());
        }
        vec4 operator/(vec4 t_other){
            return vec4(this->getX() / t_other.getX(), this->getY() / t_other.getY(), this->getZ() / t_other.getZ(), this->getW() / t_other.getW());
        }
        constexpr _FloatType accumulate(){
            return this->getX() + this->getY() + this->getZ() + this->getW();
        }
#endif

        constexpr _ArithmeticType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float dot(vec4<_VectorType> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr vec4<_ArithmeticType> normalize(){
            _ArithmeticType m_magnitude = this->getLength();
            return (*this) / vec4<_ArithmeticType>{m_magnitude};
        }
        constexpr uint8_t getDepth(){
            return 4;
        }
        std::string toString(){
            return std::string("vec4("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ","s + std::to_string(this->getZ()) + ","s + std::to_string(this->getW()) + ")"s);
        }
    };
}
#endif