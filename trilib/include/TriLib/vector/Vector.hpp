#ifndef VECTOR_HPP_
#define VECTOR_HPP_
#include <iostream>
#include <type_traits>
#include <emmintrin.h>
#ifdef __AVX__
#include <immintrin.h>
#endif

namespace tl{
    using namespace std::string_literals;
    struct vec{
        vec(){}
        vec(const vec&) = default;
        ~vec(){}

        constexpr uint8_t getLength(){
            return 0;
        }

        vec& operator=(const vec&){
            return *this;
        }

    };

    template<class _FloatType> requires std::is_floating_point<_FloatType>::value
    struct vec1: public vec{
    private:
        _FloatType m_x;
    public:
        vec1(_FloatType t_x): m_x(t_x){}
        vec1(const vec1&) = default;
        vec1(){}
        ~vec1();

        inline _FloatType getX(){
            return this->m_x;
        }        
        vec1        operator+(vec1 t_other){
            return vec1(this->getX() + t_other.getX());
        }
        vec1        operator-(vec1 t_other){
            return vec1(this->getX() - t_other.getX());
        }
        vec1        operator*(vec1 t_other){
            return vec1(this->getX() * t_other.getX());
        }
        vec1        operator/(vec1 t_other){
            return vec1(this->getX() / t_other.getX());
        }
        constexpr uint8_t getLength(){
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
        vec2(_FloatType t_x, _FloatType t_y): m_x(t_x), m_y(t_y){}
        vec2(const vec2&) = default;
        vec2(){}
        ~vec2(){}

        inline _FloatType getX(){
            return this->m_x;
        }
        inline _FloatType getY(){
            return this->m_y;
        }

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
        constexpr uint8_t getLength(){
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
        vec3(_FloatType t_x, _FloatType t_y, _FloatType t_z): m_x(t_x), m_y(t_y), m_z(t_z){}
        vec3(const vec3&) = default;
        vec3(){}
        ~vec3(){}
        
        inline _FloatType getX(){
            return this->m_x;
        }
        inline _FloatType getY(){
            return this->m_y;
        }
        inline _FloatType getZ(){
            return this->m_z;
        }
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
#else
        static inline __m128d loadToSSERegister(vec3<double> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline __m128d loadToSSERegister(vec3<float> t_vector){
            return _mm_setr_pd(t_vector.getX(), t_vector.getY());
        }
        static inline vec3 loadToVec3(__m128d t_sseRegister, double t_z){
            return vec3(t_sseRegister[0], t_sseRegister[1], t_z);
        }
#endif

        vec3        operator+(vec3 t_other){
#ifdef __AVX__
            return loadToVec3(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec3(_mm_add_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() + t_other.getZ());
#endif
        }
        vec3        operator-(vec3 t_other){
#ifdef __AVX__
            return loadToVec3(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec3(_mm_sub_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() - t_other.getZ());
#endif
        }
        vec3        operator*(vec3 t_other){
#ifdef __AVX__
            return loadToVec3(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec3(_mm_mul_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() * t_other.getZ());
#endif
        }
        vec3        operator/(vec3 t_other){
#ifdef __AVX__
            return loadToVec3(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec3(_mm_div_pd(loadToSSERegister(*this), loadToSSERegister(t_other)), this->getZ() / t_other.getZ());
#endif
        }
        constexpr uint8_t getLength(){
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
#else
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
#endif

        vec4        operator+(vec4 t_other){
#ifdef __AVX__
            return loadToVec4(_mm256_add_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec4(_mm_add_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_add_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
#endif
        }
        vec4        operator-(vec4 t_other){
#ifdef __AVX__
            return loadToVec4(_mm256_sub_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec4(_mm_sub_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_sub_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
#endif
        }
        vec4        operator*(vec4 t_other){
#ifdef __AVX__
            return loadToVec4(_mm256_mul_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec4(_mm_mul_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_mul_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
#endif
        }
        vec4        operator/(vec4 t_other){
#ifdef __AVX__
            return loadToVec4(_mm256_div_pd(loadToAVXRegister(*this), loadToAVXRegister(t_other)));
#else
            return loadToVec4(_mm_div_pd(loadToFirstSSERegister(*this), loadToFirstSSERegister(t_other)), _mm_div_pd(loadToSecondSSERegister(*this), loadToSecondSSERegister(t_other)));
#endif
        }
        constexpr uint8_t getLength(){
            return 4;
        }
        std::string toString(){
            return std::string("vec4("s + std::to_string(this->getX()) + ","s + std::to_string(this->getY()) + ","s + std::to_string(this->getZ()) + ","s + std::to_string(this->getW()) + ")"s);
        }
    };
}
#endif