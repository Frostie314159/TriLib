/*
Written by Frostie314159 in 2021
*/

#ifndef VECTOR_HPP_
#define VECTOR_HPP_
#include <iostream>
#include <type_traits>
#include <cmath>
#undef __SSE2__
#ifdef __AVX__
#include <immintrin.h>
#elif defined(__SSE2__)
#include <emmintrin.h>
#endif

namespace tl{
    using namespace std::string_literals;
    struct vec{
        vec(){}
        vec(const vec&) = default;
        ~vec(){}

        constexpr uint8_t getDepth(){
            return 0;
        }
        vec& operator=(const vec&){
            return *this;
        }
        float accumulate(){
            return 0;
        }

    };
    //Template used to make switching between precision levels easier.
    template<class _FloatType> requires std::is_floating_point<_FloatType>::value
    struct vec1: public vec{
    private:
        _FloatType m_x;
    public:
        //Constructor and Destructor Section
        vec1(_FloatType t_x): m_x(t_x){}
        vec1(const vec1&) = default;
        vec1(){}
        ~vec1();
        
        //Getter and Setter Section
        constexpr _FloatType getX(){
            return this->m_x;
        }
        constexpr void setX(_FloatType t_x){
            this->m_x = t_x;
        }

        //Operator overloading section
        vec1 operator+(vec1 t_other){
            return vec1(this->getX() + t_other.getX());
        }
        vec1 operator-(vec1 t_other){
            return vec1(this->getX() - t_other.getX());
        }
        vec1 operator*(vec1 t_other){
            return vec1(this->getX() * t_other.getX());
        }
        vec1 operator/(vec1 t_other){
            return vec1(this->getX() / t_other.getX());
        }

        //Advanced Maths Section
        constexpr _FloatType getLength(){
            //fast way to get the length of a 1D vector.
            if(this->getX() < 0)
                return -1;
            else if(this->getX() > 0)
                return 1;
            return 0;
        }
        constexpr _FloatType accumulate(){
            //Adding up the components of the vector is obsolete here.
            return this->getX();
        }
        constexpr vec1<_FloatType> normalize(){
            //Using the actual algorithm doesn't make sense here
            //since the vector is one Dimensional and the algorithm
            //will either return -1,0 or 1.
            return vec1<_FloatType>(this->getLength());
        
        }
        template<class T> requires std::is_floating_point<T>::value
        constexpr float dot(vec1<T> t_other){
            //Dot product algorithm x1*x2.
            return (*this * t_other).accumulate();
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 1;
        }
        std::string to_string(){
            return std::string("vec1("s + std::to_string(this->getX()) + ")"s);
        }
    };

    template<class _FloatType> requires std::is_floating_point<_FloatType>::value
    struct vec2: public vec{
    private:
        _FloatType m_x, m_y;
    public:
        //Constructor and Destructor Section
        vec2(_FloatType t_x, _FloatType t_y): m_x(t_x), m_y(t_y){}
        vec2(_FloatType t_x): m_x(t_x), m_y(t_x){}
        vec2(const vec2&) = default;
        vec2(){}
        ~vec2(){}

        //Getter and Setter Section
        constexpr _FloatType getX(){
            return this->m_x;
        }
        constexpr _FloatType getY(){
            return this->m_y;
        }
        constexpr void setX(_FloatType t_x){
            this->m_x = t_x;
        }
        constexpr void setY(_FloatType t_y){
            this->m_y = t_y;
        }

        //Operator overloading is SSE optimized
#ifdef __SSE2__
        static inline __m128d loadToSSERegister(vec2<double> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline __m128d loadToSSERegister(vec2<float> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline vec2 loadToVec2(__m128d t_sseRegister){
            return vec2(t_sseRegister[0], t_sseRegister[1]);
        }
        
        vec2 operator+(vec2 t_other){
            return loadToVec2(_mm_add_pd(loadToSSERegister(*this),loadToSSERegister(t_other)));
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

        constexpr _FloatType accumulate(){
            //Adding X and Y isn't obsolete here due to the Vector being multidimensional.
            return this->getX() + this->getY();
        }
        constexpr _FloatType getLength(){
            //Fast Way can't be used here so we need to use sqrt(x²+y²).
            //Depending on the host system this will be SSE optimized.
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        template<class T> requires std::is_floating_point<T>::value
        constexpr float dot(vec2<T> t_other){
            //Due to Operator overloading this function is actually always exactly the same.
            return (*this * t_other).accumulate();
        }

        constexpr vec2<_FloatType> normalize(){
            //Updated normalization function.
            //If the Host System supports SSE this will be optimized with said Extension.
            _FloatType m_magnitude = this->getLength(); 
            return *this / vec2<_FloatType>{m_magnitude};
        }
        constexpr uint8_t getDepth(){
            return 2;
        }
        std::string toString(){
            return std::string("vec2("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ")"s);
        }
    };
    
    template<class _FloatType> requires std::is_floating_point<_FloatType>::value
    struct vec3: public vec{
    private:
        _FloatType m_x, m_y, m_z;
    public:
        //Constructor and Destructor section
        vec3(_FloatType t_x, _FloatType t_y, _FloatType t_z): m_x(t_x), m_y(t_y), m_z(t_z){}
        vec3(_FloatType t_x): m_x(t_x), m_y(t_x), m_z(t_x){}
        vec3(const vec3&) = default;
        vec3(){}
        ~vec3(){}
        
        //Getter and Setter section
        constexpr _FloatType getX(){
            return this->m_x;
        }
        constexpr _FloatType getY(){
            return this->m_y;
        }
        constexpr _FloatType getZ(){
            return this->m_z;
        }
        constexpr void setX(_FloatType t_x){
            this->m_x = t_x;
        }
        constexpr void setY(_FloatType t_y){
            this->m_y = t_y;
        }
        constexpr void setZ(_FloatType t_z){
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
        constexpr _FloatType accumulate(){
            return this->getX() + this->getY() + this->getZ();
        }
        //The functions getLength() and dot(vec3<T>) are almost completely unchanged due to operator overloading
        constexpr _FloatType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        template<class T> requires std::is_floating_point<T>::value
        constexpr float dot(vec3<T> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr vec3<_FloatType> normalize(){
            _FloatType m_magnitude = this->getLength();
            return (*this) / vec3{m_magnitude};
        }
        constexpr uint8_t getDepth(){
            return 3;
        }
        std::string toString(){
            return std::string("vec3("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ","s + std::to_string(this->getZ()) + ")"s);
        }
    };
    
    template<class _FloatType> requires std::is_floating_point<_FloatType>::value
    struct vec4: public vec{
    private:
        _FloatType m_x, m_y, m_z, m_w;
    public:
        vec4(_FloatType t_x, _FloatType t_y, _FloatType t_z, _FloatType t_w): m_x(t_x), m_y(t_y), m_z(t_z), m_w(t_w){}
        vec4(_FloatType t_x): m_x(t_x), m_y(t_x), m_z(t_x), m_w(t_x){}
        vec4(const vec4&) = default;
        vec4(){}
        ~vec4(){}
        
        inline _FloatType getX(){
            return this->m_x;
        }
        inline _FloatType getY(){
            return this->m_y;
        }
        inline _FloatType getZ(){
            return this->m_z;
        }
        inline _FloatType getW(){
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
#endif

        constexpr _FloatType accumulate(){
            //vec2 is used here to avoid manual SSE optimization
            return (vec2<_FloatType>{this->getX(),this->getY()}).accumulate() + (vec2<_FloatType>{this->getZ(), this->getW()}).accumulate();
        }
        constexpr _FloatType getLength(){
            return std::sqrt(((*this) * (*this)).accumulate());
        }
        template<class T> requires std::is_floating_point<T>::value
        constexpr float dot(vec4<T> t_other){
            return (*this * t_other).accumulate();
        }
        constexpr vec4<_FloatType> normalize(){
            _FloatType m_magnitude = this->getLength();
            return (*this) / vec4<_FloatType>{m_magnitude};
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