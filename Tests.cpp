#include <iostream>

#include "TriLib/vector/Vector.hpp"

int main(){
    tl::vec4<float> m_vec = tl::vec4<float>{2,0,1.55556f,10};
    std::cout << (m_vec / tl::vec4<float>{0,3,4,20}).toString() << std::endl;
}