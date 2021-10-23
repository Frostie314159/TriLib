#ifndef BASE_VECTOR_HPP_
#define BASE_VECTOR_HPP_

#include <cstdint>

namespace tl{
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
}
#endif