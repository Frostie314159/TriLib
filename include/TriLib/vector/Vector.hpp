#include <iostream>
#include <emmintrin.h>
namespace tl{
    struct vec{
        vec(){

        }
        vec(const vec&) = default;
        virtual         ~vec();

        virtual vec     operator+(vec t_other);
        virtual vec     operator-(vec t_other);
        virtual vec     operator*(vec t_other);
        virtual vec     operator/(vec t_other);
        virtual bool    operator<=>(vec t_other);
        virtual uint8_t getLength();

        vec& operator=(const vec&);

    };
}