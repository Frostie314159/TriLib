#ifndef VEC_1_HPP_
#define VEC_1_HPP_

#include <type_traits>
#include <string>
#include <cstdint>

#include "TriLib/vector/Base_Vector.hpp"
#include "TriLib/util/TemplateUtil.hpp"

namespace tl{
    using std::string_literals::operator""s;
    //Template used to make switching between precision levels easier.
    ARITHMETIC_TEMPLATE(_ArithmeticType)
    struct vec1: public vec{
    private:
        _ArithmeticType m_x;
    public:
        //Constructor and Destructor Section
        vec1(_ArithmeticType t_x): m_x(t_x){}
        vec1(const vec1&) = default;
        vec1(){}
        ~vec1(){}
        
        //Getter and Setter Section
        constexpr _ArithmeticType getX(){
            return this->m_x;
        }
        constexpr void setX(_ArithmeticType t_x){
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
        constexpr _ArithmeticType getLength(){
            //fast way to get the length of a 1D vector.
            if(this->getX() < 0)
                return -1;
            else if(this->getX() > 0)
                return 1;
            return 0;
        }
        constexpr _ArithmeticType accumulate(){
            //Adding up the components of the vector is obsolete here.
            return this->getX();
        }
        constexpr vec1<_ArithmeticType> normalize(){
            //Using the actual algorithm doesn't make sense here
            //since the vector is one Dimensional and the algorithm
            //will either return -1,0 or 1.
            return vec1<_ArithmeticType>(this->getLength());
        
        }
        ARITHMETIC_TEMPLATE(_ValueType)
        constexpr float dot(vec1<_ValueType> t_other){
            //Dot product algorithm x1*x2.
            return (*this * t_other).accumulate();
        }
        //Casting section
        ARITHMETIC_TEMPLATE(_CastVectorType)
        explicit operator vec1<_CastVectorType>(){
            return vec1<_CastVectorType>(static_cast<_CastVectorType>(this->getX()));
        }
        //Util Section
        constexpr uint8_t getDepth(){
            return 1;
        }
        std::string to_string(){
            return std::string("vec1("s + std::to_string(this->getX()) + ")"s);
        }
    };
}
#endif