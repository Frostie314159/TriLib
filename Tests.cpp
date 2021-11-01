#include <iostream>
#include <chrono>

#include "TriLib/vector/vector.hpp"
template<typename T>
void testVector(){
    std::chrono::high_resolution_clock::time_point m_begin;
    double m_duration = 0;
    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        T m_vec = T{1} + T{2};
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average addition time: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        T m_vec = T{1} - T{2};
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average subtraction time: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        T m_vec = T{1} * T{2};
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average multiplication time: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        T m_vec = T{1} / T{2};
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average division time: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        float m_vec = T{1}.getLength();
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average time to get vector Length: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        float m_vec = T{1}.accumulate();
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average time to accumulate the vector: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        float m_vec = T{1}.dot(T{2});
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average time to get vector dot product: " << m_duration/1000000 << std::endl;
    m_duration = 0;

    for(int i=0; i<1000000; i++){
        m_begin = std::chrono::high_resolution_clock::now();
        T m_vec = T{1}.normalise();
        m_duration += (std::chrono::high_resolution_clock::now() - m_begin).count(); 
    }
    std::cout << "Average time to get normalise the vector: " << m_duration/1000000 << std::endl;
    m_duration = 0;
}
int main(){
    std::cout << std::boolalpha;
    std::cout << "TriLibTests" << std::endl;
    std::cout << "===========" << std::endl << std::endl;
    std::cout << "Averages are calculated with 1 milion iterations." << std::endl;
    bool m_isAVXEnabled = false;
    bool m_isSSE2Enabled = false;
#ifdef __AVX__
    m_isAVXEnabled = true;
#endif
#ifdef __SSE2__
    m_isSSE2Enabled = true;
#endif
    std::cout << "Is AVX Enabled: " << m_isAVXEnabled << std::endl;
    std::cout << "Is SSE2 Enabled: " << m_isSSE2Enabled << std::endl << std::endl;
    std::cout << "vec1" << std::endl;
    std::cout << "===" << std::endl;
    testVector<tl::vec1>();
    std::cout << "vec2" << std::endl;
    std::cout << "===" << std::endl;
    testVector<tl::vec2>();
    std::cout << "vec3" << std::endl;
    std::cout << "===" << std::endl;
    testVector<tl::vec3>();
    std::cout << "vec4" << std::endl;
    std::cout << "===" << std::endl;
    testVector<tl::vec4>();
    return 0;
}