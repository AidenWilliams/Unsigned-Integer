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
template<unsigned short base>
class MyUint {
private:
   void verify_msb(){
        //verify msb
        msb = 0;
        for(;msb < base - 1 && !uint[msb];msb++);
    }
public:
    unsigned short msb = 0;
    //since I would have been storing ether a 0/1 using the special implementation of vector<bool> is better
    std::vector<bool> uint;
    constexpr bool isPowerOf2(unsigned short n){
        if(n>0){
            while(n%2 == 0)
                n/=2;

            if(n == 1)
                return true;
        }

        return false;
    }
    explicit MyUint() : uint(std::vector<bool>({false})), msb(0) {};
    explicit MyUint(std::vector<bool> vint){
        std::vector<bool> x(base - vint.size(), false);
        copy(vint.begin(),vint.end(),back_inserter(x));
        uint = x;
        verify_msb();
    };
    // Copy Constructor
    MyUint(const MyUint & myUint) : uint(myUint.uint), msb(myUint.msb) {};
    // Move Constructor
    MyUint(MyUint && myUint)  noexcept : uint{ myUint.uint }, msb{ myUint.msb } {};

    MyUint& operator= (const MyUint & myUint) = default ;
    MyUint& operator= ( MyUint&& ) noexcept = default ;

    explicit MyUint(unsigned long long x) {
        if(!isPowerOf2(base)) throw std::bad_alloc();

        if(x > (unsigned long long)pow(2, base)) throw std::exception();
        //resize vector to base
        uint.resize(base, false);
        //convert number to binary
        int i = base - 1;
        for(; x > 0; i--){
            uint[i] = x % 2;
            x = x/2;
        }
        msb = i + 1;

    }
    /*
     * Assigner
     */
    MyUint& operator=(unsigned long long y) {
        *this =  MyUint<base> (y);
        return (*this);
    }
//    MyUint& operator=(const MyUint<base>& y) {
//        uint =  y.uint;
//        msb =  y.msb;
//        return *this;
//    }
    /*
     * Bitwise operations
     */
    //Not
    [[nodiscard]]MyUint operator~() {
        MyUint<base> copy(*this);
        for(auto bit: copy.uint){
            bit = !bit;
        }
        copy.verify_msb();
        return copy;
    }
    //And
    template<unsigned short base2>
    MyUint& operator&=(const MyUint<base2>& y) {
        int i = base - 1, j = base2 - 1;
        //Only loop until i msb or j msb
        // as anything after the msb of either of these is guaranteed to be 0
        for(; i >= msb && j >= y.msb; i--,j--)
            uint[i] = uint[i] & y.uint[j];

        //if i isnt 0 make sure everything from
        while(i >= msb)
            uint[i--] = false;

        verify_msb();
        return (*this);
    }
    //Or
    template<unsigned short base2>
    MyUint& operator|=(const MyUint<base2>& y) {
        int i = base - 1, j = base2 - 1;
        //Only loop until i msb or j msb
        // as anything after the msb of either of these is stays the same
        for(; i >= msb && j >= y.msb; i--,j--)
            uint[i] = uint[i] | y.uint[j];

        verify_msb();
        return (*this);
    }

    //XOR
    template<unsigned short base2>
    MyUint& operator^=(const MyUint<base2>& y) {
        int i = base - 1, j = base2 - 1;
        //still loop till msbs
        for(; i >= msb && j >= y.msb; i--,j--)
            uint[i] = uint[i] ^ y.uint[j];
        // if i isnt 0 then xor the remaining bits with 0
        for(;i >= msb;i--)
            uint[i] = uint[i] ^ (false);

        verify_msb();
        return (*this);
    }

    template<unsigned short base2>
    [[nodiscard]] MyUint operator& (const MyUint<base2>& y) const{ return MyUint(*this) &= y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator|(const MyUint<base2>& y) const { return MyUint(*this) |= y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator^(const MyUint<base2>& y) const { return MyUint(*this) ^= y; }

    //shift left
    MyUint& operator<<=(const uint32_t x) {
        int new_msb = msb - x;
        new_msb < 0 ? new_msb = -new_msb : new_msb = new_msb;
        std::vector<bool> z (new_msb, false);
        copy(uint.begin() + msb,uint.end(),back_inserter(z));
        z.resize(base, false);
        uint = z;
        verify_msb();
        return (*this);
    }
    //shift right
    MyUint& operator>>=(const uint32_t x) {
        int new_msb = msb + x;
        std::vector<bool> z(new_msb, false);
        copy(uint.begin() + msb,uint.end(),back_inserter(z));
        z.resize(base, false);
        uint = z;
        verify_msb();
        return (*this);
    }
    [[nodiscard]] MyUint operator<<(const uint32_t x) const { return MyUint(*this) <<= x; }
    [[nodiscard]] MyUint operator>>(const uint32_t x) const { return MyUint(*this) >>= x; }
    //bool
    template<unsigned short base2>
    [[nodiscard]] bool operator==(const MyUint<base2>& y) const {
        //if bases arent equal make sure to equalize them
        int check = base - base2;
        if(check != 0){
            //if check is negative than base2 is larger hence resize this
            if(check < 0){
                MyUint<base2> temp (this->uint);
                return temp.uint == y.uint;
            }else{
                MyUint<base> temp (y.uint);
                return uint == temp.uint;
            }

        }else
            //if they are equal
            return uint == y.uint;
    }
    template<unsigned short base2>
    [[nodiscard]] bool operator!=(const MyUint<base2>& y) const {  return !((*this )== y); }
    template<unsigned short base2>
    [[nodiscard]] bool operator<(const MyUint<base2>& y) const {
        if((*this) == y) return false;
        if ((base - msb) == (base2 - y.msb)){
            int i = msb, j = y.msb;
            for(;i < base && j < base2;i++,j++){
                if(uint[i] && y.uint[j] || !uint[i] && !y.uint[j])
                    continue;

                if(!uint[i] && y.uint[j])
                    return true;

                if(uint[i] &&!y.uint[j])
                    return false;
            }
        }
        else
            return (base - msb) < (base2 - y.msb);
    }
    template<unsigned short base2>
    [[nodiscard]] bool operator>(const MyUint<base2>& y) const { return y < (*this); }
    template<unsigned short base2>
    [[nodiscard]] bool operator<=(const MyUint<base2>& y) const { return (*this) < y || (*this) == y; }
    template<unsigned short base2>
    [[nodiscard]] bool operator>=(const MyUint<base2>& y) const { return (*this) > y || (*this) == y; }
    //ull
    [[nodiscard]] bool operator==(const unsigned long long x) const {
        MyUint<base> y(x);
        return (*this) == y;
    }
    [[nodiscard]] bool operator!=(const unsigned long long x) const {
        MyUint<base> y(x);
        return (*this) != y;
    }
    [[nodiscard]] bool operator<(const unsigned long long x) const {
        MyUint<base> y(x);
        return y > (*this);
    }
    [[nodiscard]] bool operator>(const unsigned long long x) const {
        MyUint<base> y(x);
        return y < (*this);
    }
    [[nodiscard]] bool operator<=(const unsigned long long x) const {
        MyUint<base> y(x);
        return (*this) < y || (*this) == y; }
    [[nodiscard]] bool operator>=(const unsigned long long x) const {
        MyUint<base> y(x);
        return (*this) > y || (*this) == y;
    }

    template<unsigned short base2>
    MyUint& operator+=(const MyUint<base2>& y) {
        MyUint<base> keep = (MyUint(*this) &  y) << 1;
        MyUint<base> res =  MyUint(*this) ^ y;
        if (keep == 0){
            *(this) = res;
            return *(this);
        }
        keep += res;
        *(this) = keep;
        return *(this);
    }

    template<unsigned short base2>
    MyUint& operator-=(const MyUint<base2>& y) {
        // undefined negative behaviour
        if ((*this) < y) throw std::exception();
        MyUint<base> y2(y.uint);
        MyUint<base> borrow;
        while (y2 != 0){
            borrow = (~*(this)) & y2;
            (*this) ^= y2;
            borrow <<= 1;
            y2 = borrow;
        }
        return *(this);
    }

    //https://www.sciencedirect.com/topics/engineering/binary-multiplication
    template<unsigned short base2>
    MyUint& operator*=(const MyUint<base2>& y) {
        MyUint<base2> temp_number;
        MyUint<base> result(0);
        for(int i = base2 - 1;i >= 0 && i >= y.msb - 1; i--){
            if(y.uint[i]){
                temp_number = MyUint<base2>(this->uint);
                temp_number <<= (base2 - i - 1);
                result += temp_number;
            }
        }
        this->uint = result.uint;
        this->msb = result.msb;
        return (*this);
    }

    template<unsigned short base2>
    MyUint& operator/=(const MyUint<base2>& y) {
        if(y == 0) throw std::runtime_error("Math error: Attempted to divide by Zero\n");
        // Quotient extracted so far
        MyUint<base> quotient(0);
        // intermediate remainder
        MyUint<base> rem(0);
        for(int i = 0; i < base; i++){
            rem <<= 1;
            rem.uint[base-1] = uint[i];
            for(;rem.msb < base - 1 && !rem.uint[rem.msb];rem.msb++);
            if(rem >= y){
                rem -= y;
                quotient.uint[i] = true;
                for(;quotient.msb < base - 1 && !quotient.uint[quotient.msb];quotient.msb++);
            }
        }
        msb = 0;
        for(;msb < base - 1 && !quotient.uint[msb];msb++);
        this->uint = quotient.uint;
        return *(this);
    }

    template<unsigned short base2>
    MyUint& operator%=(const MyUint<base2>& y) {
        if(y == 0) throw std::runtime_error("Math error: Attempted to divide by Zero\n");
        // Quotient extracted so far
        MyUint<base> quotient(0);
        // intermediate remainder
        MyUint<base> rem(0);
        for(int i = 0; i < base; i++){
            rem <<= 1;
            rem.uint[base-1] = uint[i];
            for(;rem.msb < base - 1 && !rem.uint[rem.msb];rem.msb++);
            if(rem >= y){
                rem -= y;
                quotient.uint[i] = true;
                for(;quotient.msb < base - 1 && !quotient.uint[quotient.msb];quotient.msb++);
            }
        }
        msb = 0;
        for(;msb < base - 1 && !rem.uint[msb];msb++);
        this->uint = rem.uint;
        return *(this);
    }

    template<unsigned short base2>
    [[nodiscard]] MyUint operator+(const MyUint<base2>& y) const { return MyUint(*this) += y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator-(const MyUint<base2>& y) const { return MyUint(*this) -= y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator*(const MyUint<base2>& y) const { return MyUint(*this) *= y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator/(const MyUint<base2>& y) const { return MyUint(*this) /= y; }
    template<unsigned short base2>
    [[nodiscard]] MyUint operator%(const MyUint<base2>& y) const { return MyUint(*this) %= y; }

    MyUint operator++(int){
        MyUint<base> old(*this);
        (*this) += 1;
        return old;
    }
    MyUint operator++() {
        (*this) += 1;
        return (*this);
    }

    MyUint operator--(int){
        MyUint<base> old(*this);
        (*this) -= 1;
        return old;
    }
    MyUint operator--() {
        (*this) -= 1;
        return (*this);
    }

    [[nodiscard]] MyUint operator+(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) += y;
    }
    MyUint& operator+=(const unsigned long long x) {
        MyUint<base> y(x);
        *this += y;
        return (*this);
    }
    [[nodiscard]] MyUint operator-(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) -= y;
    }
    MyUint& operator-=(const unsigned long long x) {
        MyUint<base> y(x);
        *this -= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator*(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) *= y;
    }
    MyUint& operator*=(const unsigned long long x) {
        MyUint<base> y(x);
        *this *= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator/(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) /= y;
    }
    MyUint& operator/=(const unsigned long long x) {
        MyUint<base> y(x);
        *this /= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator%(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) %= y;
    }
    MyUint& operator%=(const unsigned long long x) {
        MyUint<base> y(x);
        *this %= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator&(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) &= y;
    }
    MyUint& operator&=(const unsigned long long x) {
        MyUint<base> y(x);
        *this &= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator|(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) |= y;
    }
    MyUint& operator|=(const unsigned long long x) {
        MyUint<base> y(x);
        *this |= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator^(const unsigned long long x) const {
        MyUint<base> y(x);
        return MyUint(*this) ^= y;
    }
    MyUint& operator^=(const unsigned long long x) {
        MyUint<base> y(x);
        *this ^= y;
        return (*this);
    }

    template<typename type>
    [[nodiscard]] type convertTo(){
        size_t s = sizeof(type);
        if(base - msb > sizeof(type) * 8) throw std::bad_cast();
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
                    // Add character 1 to the "divided String"
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