#ifndef VEC_2_HPP_
#define VEC_2_HPP_

#include <type_traits>
#include <string>
#include <cstdint>
#include <cmath>

//#include "TriLib/Util/SSEUtil.hpp"
#include "TriLib/util/TemplateUtil.hpp"
#include "TriLib/vector/Base_Vector.hpp"

namespace tl{
    using std::string_literals::operator""s;
    //Template means: _ArithmeticType must be of arithmetic type and must not be constant
    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct vec2: public vec{
    private:
        _ArithmeticType m_x, m_y;
    public:
        //Constructor and Destructor Section
        vec2(_ArithmeticType t_x, _ArithmeticType t_y): m_x(t_x), m_y(t_y){}
        vec2(_ArithmeticType t_x): m_x(t_x), m_y(t_x){}
        vec2(const vec2&) = default;
        vec2(){}
        ~vec2(){}

        //Getter and Setter Section
        constexpr _ArithmeticType getX(){
            return this->m_x;
        }
        constexpr _ArithmeticType getY(){
            return this->m_y;
        }
        constexpr void setX(_ArithmeticType t_x){
            this->m_x = t_x;
        }
        constexpr void setY(_ArithmeticType t_y){
            this->m_y = t_y;
        }

        //Operator overloading is SSE optimized
#ifdef __SSE2__
        ARITHMETIC_TEMPLATE(_VectorType)
        static inline tl::detail::RegisterType<_VectorType> loadToSSERegister(vec2<_VectorType> t_vector){
            if constexpr(std::is_same<float, _VectorType>::value)
                return _mm_setr_ps(t_vector.getX(), t_vector.getY(), 0, 0);
            else if constexpr(std::is_same<double, _VectorType>::value)
                return _mm_setr_pd(t_vector.getX(), t_vector.getY());
            else if constexpr(std::is_same<int32_t, _VectorType>::value || std::is_same<uint32_t, _VectorType>::value)
                return _mm_setr_pi32(t_vector.getX(), t_vector.getY());
            else
                return _mm_setr_epi64(static_cast<__m64>(t_vector.getX()), static_cast<__m64>(t_vector.getY()));

        }
        SSE_REGISTER_TEMPLATE(_SSERegisterPrimitiveType)
        static inline vec2<tl::detail::RegisterType<_SSERegisterPrimitiveType>> loadToVec2(_SSERegisterPrimitiveType t_sseRegister){
            return vec2<tl::detail::RegisterType<_SSERegisterPrimitiveType>>(t_sseRegister[0], t_sseRegister[1]);
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        vec2 operator+(vec2<_VectorType> t_other){
            if constexpr((std::is_floating_point<_ArithmeticType>::value && std::is_floating_point<_VectorType>::value) || (std::is_integral<_ArithmeticType>::value && std::is_integral<_VectorType>::value)){
                if constexpr()
            }else{

            }
        }
        vec2 operator-(vec2 t_other){
            return loadToVec2(_mm_sub_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
        vec2 operator*(vec2 t_other){
            return loadToVec2(_mm_mul_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
        vec2 operator/(vec2 t_other){
            return loadToVec2(_mm_div_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
        }
#else
        vec2 operator+(vec2 t_other){
            return vec2(this->getX() + t_other.getX(), this->getY() + t_other.getY());
        }
        vec2 operator-(vec2 t_other){
            return vec2(this->getX() - t_other.getX(), this->getY() - t_other.getY());
        }
        vec2 operator*(vec2 t_other){
            return vec2(this->getX() * t_other.getX(), this->getY() * t_other.getY());
        }
        vec2 operator/(vec2 t_other){
            return vec2(this->getX() / t_other.getX(), this->getY() / t_other.getY());
        }
#endif

        constexpr _ArithmeticType accumulate(){
            //Adding X and Y isn't obsolete here due to the Vector being multidimensional.
            return this->getX() + this->getY();
        }
        constexpr _ArithmeticType getLength(){
            //Fast Way can't be used here so we need to use sqrt(x²+y²).
            //Depending on the host system this will be SSE optimized.
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float dot(vec2<_VectorType> t_other){
            //Due to Operator overloading this function is actually always exactly the same.
            return (*this * t_other).accumulate();
        }

        constexpr vec2<_ArithmeticType> normalize(){
            //Updated normalization function.
            //If the Host System supports SSE this will be optimized with said Extension.
            _ArithmeticType m_magnitude = this->getLength(); 
            return *this / vec2<_ArithmeticType>{m_magnitude};
        }
        constexpr uint8_t getDepth(){
            return 2;
        }
        std::string toString(){
            return std::string("vec2("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ")"s);
        }
    };
}
#endif