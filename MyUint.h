//
// Created by aiden on 23/12/2020.
//
#include "vector"
#include <climits>
#include <cmath>
#include <string>
#include <stdexcept>


#ifndef BIGINT_MYUINT_H
#define BIGINT_MYUINT_H
#endif //BIGINT_MYUINT_H
//max 2048 anyways
template<ushort base>
class MyUint {
private:
    short bits_on(const std::vector<bool>& x){
        short bits = 0;
        for(auto y: x){
            if(y) bits++;
        }
        return bits;
    }
public:
    //u_char does not cover positions after 255!
    ushort stop = 0;
    //since I would have been storing ether a 0/1 using the special implementation of vector<bool> is better
    std::vector<bool> uint;
    constexpr bool isPowerOf2(ushort n){
        if(n>0)
        {
            while(n%2 == 0)
            {
                n/=2;
            }
            if(n == 1)
            {
                return true;
            }
        }
        return false;
    }
    explicit MyUint() : uint(std::vector<bool>({false})) {
        if(!isPowerOf2(base)) throw std::bad_alloc();
    };
    explicit MyUint(std::vector<bool>& vint) : uint(vint) {
        if(!isPowerOf2(base)) throw std::bad_alloc();
    };
    MyUint(MyUint const & myUint) : uint(myUint.uint) {
        if(!isPowerOf2(base)) throw std::bad_alloc();
    };
    explicit MyUint(unsigned long long x) {
        if(!isPowerOf2(base)) throw std::bad_alloc();
        //static_assert(!isPowerOf2(base), "Base has to be a in the form of 2^n!");
        if(x > (unsigned long long)pow(2, base)) throw std::exception();
        //resize vector to base
        uint.resize(base, false);
        //convert number to binary
        int i;
        for(i = base - 1; x > 0; i--){
            uint[i] = x % 2;
            x = x/2;
        }
        stop = ++i;
    }

    //shift left
    MyUint& operator<<=(const uint32_t x) {
        for(int j = 0; j < x; j++){
            for(int i = 0; i < uint.size(); i++){
                if(i+1 != uint.size())
                    uint[i] = uint[i+1];
                else
                    uint[i] = false;
            }
        }
        return (*this);
    }
    //shift right
    MyUint& operator>>=(const uint32_t x) {
        for(int j = 0; j < x; j++){
            for(int i = uint.size() - 1; i >= 0; i--){
                if(i-1 != -1)
                    uint[i] = uint[i-1];
                else
                    uint[i] = false;
            }
        }
    }
    MyUint operator<<(const uint32_t x) const { return MyUint(*this) <<= x; }
    MyUint operator>>(const uint32_t x) const { return MyUint(*this) >>= x; }
//    MyUint operator!() const {
//        for(bool i : uint){
//            i = !i;
//        }
//        return *this;
//    }


    template<ushort b1>
    MyUint& operator+=(MyUint<b1> b) {
        if (uint.size() < b.uint.size()) throw std::exception();
        bool carry = false;
        std::vector<bool> n {};
        std::vector<bool> bits {false,false,false};
        while (uint.size() > b.uint.size())
            b.uint.insert(b.uint.begin(), false);
        for (int i = b.uint.size() - 1; i >= 0; i--){
            bits[0] = uint[i];
            bits[1] = b.uint[i];
            bits[2] = carry;
            if(bits_on(bits) == 0){
                n.insert(n.begin(), false);
                carry = false;
            }else if(bits_on(bits) == 1){
                n.insert(n.begin(), true);
                carry = false;
            }else if(bits_on(bits) == 2){
                n.insert(n.begin(), false);
                carry = true;
            }else if(bits_on(bits) == 3){
                n.insert(n.begin(), true);
                carry = true;
            }
        }
        //keep only base bits from n
        short sizeDifference = n.size() - base;
        if(sizeDifference > 0)
            n.erase(n.begin() + (sizeDifference - 1));
        this->uint = n;
        //update this.stop?
        return *(this);
    }


    void perform_Twos_Comp(){
        uint = twos_Comp(this->uint);
    }


    MyUint twos_Comp(std::vector<bool> binary) {
        MyUint<base> ones_comp;
        for (int i = 0; i < binary.size(); i++) {
            ones_comp.uint[i] = !binary[i];
        }
        MyUint<base> one(1);
        return ones_comp += one;
    }

    //Look at this shit
    //template<ushort b1>
    MyUint& operator-=(const MyUint &b) {
        MyUint cpy(b);
        cpy.perform_Twos_Comp();
        return this+=cpy;
    }

    template<typename type>
    [[nodiscard]] type convertTo(){
        size_t s = sizeof(type);
        if(base - stop > sizeof(type) * 8) throw std::bad_cast();
        std::string binary = to_binary_string();
        type y;
        switch (s) {
            case 4:
                y = stoi(binary, nullptr, 2);
                break;
            case 8:
                y = stol(binary, nullptr, 2);
                break;
            default:
                y = -1;
        }
        return y;
    }


    [[nodiscard]] std::string to_binary_string() const {
        std::string ret;
        //for(auto it = uint.rbegin(); it != uint.rend(); it++){
        for(auto bit: uint)
            bit ? ret += "1" : ret += "0";

        return ret.empty() ? "0" : ret;
    }

    [[nodiscard]] std::string to_string() const{
        std::string binary = this->to_binary_string();
        // Here we will store the resulting output
        std::string result{};

        // The conversion will also work for other  number bases
        // For base > 10 you need to adapt the creation of the digit at the bottom
        constexpr unsigned int numberBase{ 10 };

        // So, we will perfrom an integer division by 10, until the number is 0
        do {

            // The remainder is the digit that we are interested in
            unsigned int remainder{};
            // Temporary result of integer division
            std::string dividedNumberAsString{};

            // Do the division
            for (const char bit : binary) {

                // Calculate the remainder
                remainder = remainder * 2 + (bit - '0');

                // If we have a overflow (e.g. number is bigger than 10)
                if (remainder >= numberBase) {

                    // Handle overflow
                    remainder -= numberBase;
                    // Add character 1 to the "devidedString"
                    dividedNumberAsString += "1";
                }
                else {
                    dividedNumberAsString += "0";
                }
            }
            // Now "dividedNumberAsString" as string is the result of the devision by e.g. 10 in binary form
            binary = dividedNumberAsString;
            // The remainder is the number that we are interested in
            result.insert(0, 1, '0' + remainder);

            // Continue the loop with the new binary string
        } while (std::count(binary.begin(), binary.end(), '1'));

        // Show result
        return result.empty() ? "0" : result;
    }
};