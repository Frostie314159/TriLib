#include <iostream>
#include <chrono>

#include "TriLib/vector/Vector.hpp"

int main(){
    std::chrono::high_resolution_clock::time_point m_begin;
    uint64_t m_duration = 0;
    for(int i=0; i<1000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        tl::vec2 m_result = tl::vec2{1} + tl::vec2{2};
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count();
    }
    std::cout << m_duration / 1000 << std::endl;
    return 0;
}