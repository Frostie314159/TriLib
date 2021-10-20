#include <iostream>

#include "TriLib/vector/Vector.hpp"

int main(){
    tl::vec2<double> t_vec = tl::vec2<double>{2};
    tl::vec2<double> t_vec2 = tl::vec2<double>{3,1};
    std::cout << t_vec.normalize().dot(t_vec2.normalize()) << std::endl;
    return 0;
}