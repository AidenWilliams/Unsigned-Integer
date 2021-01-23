#include <iostream>
#include "MyUint.h"

int main() {
    MyUint<8> x(12);
    MyUint<8> y(4);
//    x = x + 1;
//    x = x - 20;
//    x = x * 2;
//    //std::cout << x.to_string();
//    x = x / 2;
//    //std::cout << x.to_string();
//    !x;
    //y = !x;
//    x |= y;
    x = y;
    std::cout << x.to_string() << std::endl;
//    std::cout << (x++).to_string() << std::endl;
//    std::cout << (x).to_string() << std::endl;
//    std::cout << (--x).to_string() << std::endl;
//    std::cout << (x--).to_string() << std::endl;
//    std::cout << (x).to_string() << std::endl;
}