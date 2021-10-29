#ifndef VEC_1_HPP_
#define VEC_1_HPP_

#include <type_traits>
#include <string>
#include <cstdint>

#include "TriLib/vector/Base_Vector.hpp"
#include "TriLib/util/TemplateUtil.hpp"

namespace tl::internal{
    using std::string_literals::operator""s;
    //Template used to make switching between precision levels easier.
    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct _vec1: public vec{
        //Data Section
        _ArithmeticType x;

        //Constructor and Destructor Section
        _vec1(_ArithmeticType t_x): x(t_x){}
        _vec1(const _vec1&) = default;
        _vec1(){}
        ~_vec1(){}

        //Operator overloading section
        _vec1& operator+(const _vec1& t_other){
            return _vec1(this->x + t_other.x);
        }
        _vec1& operator-(const _vec1& t_other){
            return _vec1(this->x - t_other.x);
        }
        _vec1& operator*(const _vec1& t_other){
            return _vec1(this->x * t_other.x);
        }
        _vec1& operator/(const _vec1& t_other){
            return _vec1(this->x / t_other.x);
        }
        _vec1& operator=(const _vec1& t_other){
            if(this != &t_other){
                this->x = t_other.x;
                if(*this != t_other){
                    throw std::logic_error("Copy Assignment operator didn't work!");
                    return vec1<_ArithmeticType>(0);
                }
            }
            return *this;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator==(const _vec1<_VectorType>& t_other){
            return this->x == t_other.x;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator!=(const _vec1<_VectorType>& t_other){
            return this->x != t_other.x;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<(const _vec1<_VectorType>& t_other){
            return this->x < t_other.x;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>(const _vec1<_VectorType>& t_other){
            return this->x > t_other.x;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator<=(const _vec1<_VectorType>& t_other){
            return this->x <= t_other.x;
        }
        ARITHMETIC_TEMPLATE(_VectorType)
        const bool operator>=(const _vec1<_VectorType>& t_other){
            return this->x >= t_other.x;
        }
        
        //Casting section
        ARITHMETIC_TEMPLATE(_CastVectorType)
        operator _vec1<_CastVectorType>(){
            return _vec1<_CastVectorType>(static_cast<_CastVectorType>(this->x));
        }

        //Advanced Maths Section
        constexpr _ArithmeticType getLength(){
            //fast way to get the length of a 1D vector.
            if(this->x < 0)
                return -1;
            else if(this->x > 0)
                return 1;
            else
                return 0;
        }
        constexpr _ArithmeticType accumulate(){
            //Adding up the components of the vector is obsolete here.
            return this->x;
        }
        ARITHMETIC_TEMPLATE(_ValueType)
        constexpr float dot(_vec1<_ValueType> t_other){
            //Dot product algorithm x1*x2.
            return (*this * t_other).accumulate();
        }
        constexpr _vec1<_ArithmeticType> normalise(){
            //Using the actual algorithm doesn't make sense here
            //since the vector is one Dimensional and the algorithm
            //will either return -1,0 or 1.
            return _vec1<_ArithmeticType>(this->getLength());
        
        }

        //Util Section
        constexpr uint8_t getDepth(){
            return 1;
        }
        inline std::string to_string(){
            return std::string("vec1("s + std::to_string(this->x) + ")"s);
        }
    };
}
#endif