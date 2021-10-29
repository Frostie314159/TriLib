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
namespace tl::internal{
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
        

        //Operator overloading is SSE optimized
#ifdef __AVX__
        static inline __m256d loadToAVXRegister(_vec3<double> t_vector){
            return _mm256_setr_pd(t_vector.x, t_vector.y, t_vector.z, 0);
        }
        static inline __m256d loadToAVXRegister(_vec3<float> t_vector){
            return _mm256_setr_pd(t_vector.x, t_vector.y, t_vector.z, 0);
        }
        static inline _vec3 loadTo_vec3(__m256d t_avxRegister){
            return _vec3(t_avxRegister[0], t_avxRegister[1], t_avxRegister[2]);
        }
        _vec3 operator+(_vec3 t_other){
            return loadTo_vec3(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec3 operator-(_vec3 t_other){
            return loadTo_vec3(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec3 operator*(_vec3 t_other){
            return loadTo_vec3(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
        _vec3 operator/(_vec3 t_other){
            return loadTo_vec3(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
        }
#elif defined(__SSE2__)
        static inline __m128d loadToSSERegister(_vec3<double> t_vector){
            return _mm_setr_pd(t_vector.x, t_vector.y);
        }
        static inline __m128d loadToSSERegister(_vec3<float> t_vector){
            return _mm_setr_pd(t_vector.x, t_vector.y);
        }
        static inline _vec3 loadTo_vec3(__m128d t_sseRegister, double t_z){
            return _vec3(t_sseRegister[0], t_sseRegister[1], t_z);
        }
        _vec3 operator+(_vec3 t_other){
            return loadTo_vec3(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z + t_other.z);
        }
        _vec3 operator-(_vec3 t_other){
            return loadTo_vec3(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z - t_other.z);
        }
        _vec3 operator*(_vec3 t_other){
            return loadTo_vec3(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z * t_other.z);
        }
        _vec3 operator/(_vec3 t_other){
            return loadTo_vec3(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->z / t_other.z);
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
        _vec3& operator=(const _vec3& t_other){
            if(this != &t_other){
                this->x = t_other.x;
                this->y = t_other.y;
                this->z = t_other.z;
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return vec3<_ArithmeticType>(0);//This will never be called because an exception was previously thrown.
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
        constexpr float dot(_vec3<_VectorType> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr _vec3<_ArithmeticType> normalize(){
            _ArithmeticType m_magnitude = this->getLength();
            return (*this) / _vec3{m_magnitude};
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 3;
        }
        inline std::string toString(){
            return std::string("vec3("s + std::to_string(this->x) + ","s + std::to_string(this->y) + ","s + std::to_string(this->z) + ")"s);
        }
    };
}
#endif