#include <iostream>
#include "MyUint.h"

int main() {
    MyUint<8> x(11);
    MyUint<8> y(1);
    MyUint<8> z(4);
    x = z - y;
//    x = x + 1;
//    x = x - 20;
//    x = x * 2;
//    //std::cout << x.to_string();
//    x = x / 2;
//    //std::cout << x.to_string();
//    !x;
    //x <<= 2;
    //x = 2;
    //y = !x;
//    x |= y;
//    std::cout << (x++).to_string() << std::endl;
    std::cout << x.to_string() << std::endl;
    std::cout << y.to_string() << std::endl;
    std::cout << z.to_string() << std::endl;
}