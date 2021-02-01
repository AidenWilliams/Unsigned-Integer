#include <iostream>
#include "MyUint.h"
int main() {
    MyUint<32> y(1000);
    MyUint<16> x(10);
    x+=y;
    std::cout << y.to_string() << std::endl;
    std::cout << x.to_string() << std::endl;
    std::cout << y.to_binary_string() << std::endl;
    std::cout << x.to_binary_string() << std::endl;

    long Y = y.convertTo<long>();
    int X = x.convertTo<int>();

    std::cout << Y << std::endl;
    std::cout << X << std::endl;
}