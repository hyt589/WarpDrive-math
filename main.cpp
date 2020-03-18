#include <iostream>
#include <WarpMath.hpp>

int main(int argc, char const *argv[])
{
    WarpMath::vec4i vec1 = WarpMath::Vec4i(1,2,3,4);
    WarpMath::vec4i vec2 = WarpMath::Vec4i(4,5,6,7);
    WarpMath::vec4i vec3 =  vec1 * vec2 ;
    std::cout << vec3;
    WarpMath::Vector<int, 5> vec5 = WarpMath::Vector<int, 5>(new int[5]{5,5,5,5,5});
    std::cout << vec3.max() << "\n";
    std::cout << vec5;
    return 0;
}
