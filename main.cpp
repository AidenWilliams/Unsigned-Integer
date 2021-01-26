#include <iostream>
#include "MyUint.h"

int main() {
    MyUint<8> x(12);
    MyUint<8> y(10);
    MyUint<8> z(5);


    std::cout << (x % z).to_string() << std::endl;
    //x = ~y;
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
    std::cout << x.to_binary_string() << std::endl;
    std::cout << y.to_string() << std::endl;
    std::cout << z.to_string() << std::endl;
    x<<=5;
    std::cout << x.to_binary_string() << std::endl;
}