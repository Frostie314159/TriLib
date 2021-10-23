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
namespace tl{
    using std::string_literals::operator""s;

    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct vec3: public vec{
    private:
        _ArithmeticType m_x, m_y, m_z;
    public:
        //Constructor and Destructor section
        vec3(_ArithmeticType t_x, _ArithmeticType t_y, _ArithmeticType t_z): m_x(t_x), m_y(t_y), m_z(t_z){}
        vec3(_ArithmeticType t_x): m_x(t_x), m_y(t_x), m_z(t_x){}
        vec3(const vec3&) = default;
        vec3(){}
        ~vec3(){}
        
        //Getter and Setter section
        constexpr _ArithmeticType getX(){
            return this->m_x;
        }
        constexpr _ArithmeticType getY(){
            return this->m_y;
        }
        constexpr _ArithmeticType getZ(){
            return this->m_z;
        }
        constexpr void setX(_ArithmeticType t_x){
            this->m_x = t_x;
        }
        constexpr void setY(_ArithmeticType t_y){
            this->m_y = t_y;
        }
        constexpr void setZ(_ArithmeticType t_z){
            this->m_z = t_z;
        }

        //Operator overloading is SSE optimized
#ifdef __AVX__
        static inline __m256d loadToAVXRegister(vec3<double> t_vector){
            return _mm256_setr_pd(t_vector.getX(), t_vector.getY(), t_vector.getZ(), 0);
        }
        static inline __m256d loadToAVXRegister(vec3<float> t_vector){
            return _mm256_setr_pd(t_vector.getX(), t_vector.getY(), t_vector.getZ(), 0);
        }
        static inline vec3 loadToVec3(__m256d t_avxRegister){
            return vec3(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2]);
        }
        vec3 operator+(vec3 t_other){
            return loadToVec3(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec3 operator-(vec3 t_other){
            return loadToVec3(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec3 operator*(vec3 t_other){
            return loadToVec3(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        vec3 operator/(vec3 t_other){
            return loadToVec3(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
#elif defined(__SSE2__)
        static inline __m128d loadToSSERegister(vec3<double> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline __m128d loadToSSERegister(vec3<float> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline vec3 loadToVec3(__m128d t_sseRegister, double t_z){
            return vec3(t_sseRegister[0], t_sseRegister[1], t_z);
        }
        vec3 operator+(vec3 t_other){
            return loadToVec3(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() + t_other.getZ());
        }
        vec3 operator-(vec3 t_other){
            return loadToVec3(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() - t_other.getZ());
        }
        vec3 operator*(vec3 t_other){
            return loadToVec3(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() * t_other.getZ());
        }
        vec3 operator/(vec3 t_other){
            return loadToVec3(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() / t_other.getZ());
        }
#else
        vec3 operator+(vec3 t_other){
            return vec3(this->getX() + t_other.getX(), this->getY() + t_other.getY(), this->getZ() + t_other.getZ());
        }
        vec3 operator-(vec3 t_other){
            return vec3(this->getX() - t_other.getX(), this->getY() - t_other.getY(), this->getZ() - t_other.getZ());
        }
        vec3 operator*(vec3 t_other){
            return vec3(this->getX() * t_other.getX(), this->getY() * t_other.getY(), this->getZ() * t_other.getZ());
        }
        vec3 operator/(vec3 t_other){
            return vec3(this->getX() / t_other.getX(), this->getY() / t_other.getY(), this->getZ() / t_other.getZ());
        }
#endif
        constexpr _ArithmeticType accumulate(){
            return this->getX() + this->getY() + this->getZ();
        }
        //The functions getLength() and dot(vec3<T>) are almost completely unchanged due to operator overloading
        constexpr _ArithmeticType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        constexpr float dot(vec3<_VectorType> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr vec3<_ArithmeticType> normalize(){
            _ArithmeticType m_magnitude = this->getLength();
            return (*this) / vec3{m_magnitude};
        }
        constexpr uint8_t getDepth(){
            return 3;
        }
        std::string toString(){
            return std::string("vec3("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ","s + std::to_string(this->getZ()) + ")"s);
        }
    };
}
#endif