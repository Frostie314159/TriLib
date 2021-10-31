/*
Written by Frostie314159 in 2021
*/

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "vec1.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
namespace tl{
    //Shorter vector names
    using vec1  = tl::internal::_vec1<float>;
    using dvec1 = tl::internal::_vec1<double>;
    using ivec1 = tl::internal::_vec1<int>;

    using vec2  = tl::internal::_vec2<float>;
    using dvec2 = tl::internal::_vec2<double>;
    using ivec2 = tl::internal::_vec2<int>;

    using vec3  = tl::internal::_vec3<float>;
    using dvec3 = tl::internal::_vec3<double>;
    using ivec3 = tl::internal::_vec3<int>;

    using vec4  = tl::internal::_vec4<float>;
    using dvec4 = tl::internal::_vec4<double>;
    using ivec4 = tl::internal::_vec4<int>;
}

ARITHMETIC_TEMPLATE(_VectorType)
std::ostream& operator<<(std::ostream& t_ostream, tl::internal::_vec1<_VectorType>& t_vector){
    return t_ostream << t_vector.toString();
}
ARITHMETIC_TEMPLATE(_VectorType)
std::ostream& operator<<(std::ostream& t_ostream, tl::internal::_vec2<_VectorType>& t_vector){
    return t_ostream << t_vector.toString();
}
ARITHMETIC_TEMPLATE(_VectorType)
std::ostream& operator<<(std::ostream& t_ostream, tl::internal::_vec3<_VectorType>& t_vector){
    return t_ostream << t_vector.toString();
}
ARITHMETIC_TEMPLATE(_VectorType)
std::ostream& operator<<(std::ostream& t_ostream, tl::internal::_vec4<_VectorType>& t_vector){
    return t_ostream << t_vector.toString();
}

#endif