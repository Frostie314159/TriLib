#include <iostream>
#include <xmmintrin.h>

int main(){
    __m128d m_sse = _mm_set_pd(0.1,0.2);
    __m128d m_result = _mm_add_pd(m_sse,m_sse);
    std::cout << m_result[1] << " " << m_result[0] << std::endl;
    std::cout << 0.1 + 0.1 << " " << 0.2 + 0.2 << std::endl;
}