//
// Created by aiden on 23/12/2020.
//
#include "vector"
#include <climits>
#include <cmath>
#include <string>
#include <stdexcept>

/**
 * MyUint Class is a bit representative number class that is able to represent any unsigned integer number
 * from 0 to 2^(2^15) - 1
 * @tparam size the number of bits allocated for the number. size must be a power of 2 (e.g 2,4,8,16 etc)
 */

template<unsigned short size>
class MyUint {
private:
    /**
     * Verify MSB using this O(n) function
     */
   void verify_msb(){
        msb = 0;
        //msb is incremented in each iteration until the first on bit is found
        for(; msb < size - 1 && !bits[msb]; msb++);
    }
public:
    //msb will hold the position of the msb of the binary number here
    unsigned short msb = 0;
    //special implementation of vector<bool> allows storage of 1 bit making space representation easier to understand
    //any binary number is represented this way:
    //1010 -> bits[0] = true, bits[2] = false, bits[2] = true, bits[3] = false
    //the msb in this case would be 0
    std::vector<bool> bits;

    /**
     * returns whether a number n is a power of 2 or not
     * @param n the number being tested
     * @return bool
     */
    constexpr bool isPowerOf2(unsigned short n){
        if(n>0){
            while(n%2 == 0)
                n/=2;

            if(n == 1)
                return true;
        }

        return false;
    }

    /**
     * Standard constructor allocates size number of bits for the number
     */
    explicit MyUint() : bits(std::vector<bool>(size, false)), msb(size) {
        //The size must be a power of 2
        if(!isPowerOf2(size)) throw std::bad_alloc();
        //size also has to be smaller than 2048
        if(size > 2048) throw std::exception();
    };

    /**
     * constructor to change the vector<bool> of the number
     * @param bits the new vector<bool>
     */
    explicit MyUint(std::vector<bool> bits){
        //The size must be a power of 2
        if(!isPowerOf2(size)) throw std::bad_alloc();
        //size also has to be smaller than 2048
        if(size > 2048) throw std::exception();
        //create a new vector<bool> x and allocate size - bits.size() off bits
        //These extra bits are the difference in the sizes
        std::vector<bool> x(size - bits.size(), false);
        //copy bits into x
        copy(bits.begin(),bits.end(),back_inserter(x));
        //change bits
        this->bits = x;
        //since the object hasnt been created yet I copied the verify msb function here
        msb = 0;
        //msb is incremented in each iteration until the first on bit is found
        for(; msb < size - 1 && !bits[msb]; msb++);
    };

    // Copy Constructor
    MyUint(const MyUint & myUint) : bits(myUint.bits), msb(myUint.msb) {};
    // Move Constructor
    MyUint(MyUint && myUint)  noexcept : bits{myUint.bits }, msb{myUint.msb } {};
    /*
     * Since by having a move constructor my copy constructor was being implicitly deleted I had to explicitly
     * override the = functions to accommodate for the constructors
     */
    MyUint& operator= (const MyUint & ) = default ;
    MyUint& operator= ( MyUint&& ) noexcept = default ;

    /*
     * unsigned long long constructor that converts any ULL into a MyUint
     */
    explicit MyUint(unsigned long long x) {
        //The size must be a power of 2
        if(!isPowerOf2(size)) throw std::bad_alloc();
        //size also has to be smaller than 2048
        if(size > 2048) throw std::exception();
        //resize vector to size
        bits.resize(size, false);
        //convert number to binary
        int i = size - 1;
        for(; x > 0; i--){
            bits[i] = x % 2;
            x = x/2;
        }
        msb = i + 1;
    }

    /** *****************
     * Assignment overloading
     *
     * Takes ULL y and returns a new MyUint representing y
     * @param y an unsigned long long number
     * @return MyUint representation of y
     */
    [[nodiscard]] MyUint& operator=(unsigned long long y) {
        *this =  MyUint<size> (y);
        return (*this);
    }

    /* ******************
     * Bitwise operations
     */
    /**
     * Bitwise not for MyUint
     * @return a copy of the binary not of this
     */
    [[nodiscard]] MyUint operator~() {
        // copy this
        MyUint<size> copy(*this);
        // flip each bit in the copy
        for(auto bit: copy.bits){
            bit = !bit;
        }
        // verify the msb in in copy
        copy.verify_msb();
        // return copy
        return copy;
    }

    /**
     * Bitwise and assignment for MyUint
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this & y
     */
    template<unsigned short size2>
    MyUint& operator&=(const MyUint<size2>& y) {
        int i = size - 1, j = size2 - 1;
        //Only loop until i msb or j msb
        // as anything after the msb of either of these is guaranteed to be 0
        for(; i >= msb && j >= y.msb; i--,j--)
            // perform & on the bits
            bits[i] = bits[i] & y.bits[j];

        //if i isn't 0 make sure everything from msb to 0 is false
        while(i >= msb)
            bits[i--] = false;

        verify_msb();
        return (*this);
    }

    /**
     * Bitwise or assignment for MyUin
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this | y
     */
    template<unsigned short size2>
    MyUint& operator|=(const MyUint<size2>& y) {
        int i = size - 1, j = size2 - 1;
        //Only loop until i msb or j msb
        // as anything after the msb of either of these is stays the same
        for(; i >= msb && j >= y.msb; i--,j--)
            bits[i] = bits[i] | y.bits[j];
        // no need to verify msb as or doesnt change the msb
        return (*this);
    }

    /**
     * Bitwise xor assignment for MyUin
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this ^ y
     */
    template<unsigned short size2>
    MyUint& operator^=(const MyUint<size2>& y) {
        int i = size - 1, j = size2 - 1;
        //Only loop until i msb or j msb
        for(; i >= 0 && j >= y.msb; i--,j--)
            bits[i] = bits[i] ^ y.bits[j];
        // if i isn't 0 then xor the remaining bits with 0
        for(;i >= 0;i--)
            bits[i] = bits[i] ^ (false);

        verify_msb();
        return (*this);
    }
    // binary operator overloading for &, |, ^
    // To avoid code duplication the respective bitwise assignment operator is used on a copy of this and y
    template<unsigned short size2>
    [[nodiscard]] MyUint operator& (const MyUint<size2>& y) const{ return MyUint(*this) &= y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator|(const MyUint<size2>& y) const { return MyUint(*this) |= y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator^(const MyUint<size2>& y) const { return MyUint(*this) ^= y; }

    /**
     * left shift this MyUint by y
     * @param x the number of bits the number will be shifted to the left
     * @return this number shifter by y bits to the left
     */
    MyUint& operator<<=(const unsigned short y) {
        //calculate position of the new msb
        int new_msb = msb - y;
        //msb cannot be negative, if the new msb is less than 0 than restart from size
        new_msb < 0 ? new_msb = size + new_msb : new_msb = new_msb;
        //create a new binary number z with new_msb number of off bits
        std::vector<bool> z (new_msb, false);
        //copy the significant bits from bits and add them to the back of z
        copy(bits.begin() + msb, bits.end(), back_inserter(z));
        //fill any remaining bits with off bits
        z.resize(size, false);
        //assign z to bits
        bits = z;
        verify_msb();
        return (*this);
    }

    /**
     * right shift this MyUint by y
     * @param x the number of bits the number will be shifted to the right
     * @return this number shifter by y bits to the right
     */
    MyUint& operator>>=(const unsigned short y) {
        //calculate position of the new msb
        int new_msb = msb + y;
        //create a new binary number z with new_msb number of off bits
        std::vector<bool> z(new_msb, false);
        //copy the significant bits from bits and add them to the back of z
        copy(bits.begin() + msb, bits.end(), back_inserter(z));
        //fill any remaining bits with off bits
        z.resize(size, false);
        //assign z to bits
        bits = z;
        verify_msb();
        return (*this);
    }

    // binary shift operators
    // To avoid code duplication the respective shift assignment operator is used on a copy of this and y
    [[nodiscard]] MyUint operator<<(const unsigned short y) const { return MyUint(*this) <<= y; }
    [[nodiscard]] MyUint operator>>(const unsigned short y) const { return MyUint(*this) >>= y; }

    /* ******************
     * Boolean/Relational  operations
     */
    /**
     * Equal To operator between this and y
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether this is equal to y
     */
    template<unsigned short size2>
    [[nodiscard]] bool operator==(const MyUint<size2>& y) const {
        //if bases aren't equal make sure to equalize them
        int check = size - size2;
        if(check != 0){
            //if check is negative than size2 is larger hence resize this
            if(check < 0){
                //resize this in temp
                MyUint<size2> temp (this->bits);
                //compare the vectors
                return temp.bits == y.bits;
            }else{
                //resize y in temp
                MyUint<size> temp (y.bits);
                //compare the vectors
                return bits == temp.bits;
            }
        }else
            //if they are equal
            //compare the vectors
            return bits == y.bits;
    }

    /**
     * Not Equal To operator between this and y
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether this is not equal to y
     */
    template<unsigned short size2>
    [[nodiscard]] bool operator!=(const MyUint<size2>& y) const {  return !((*this )== y); }

    /**
     * Less than  operator between this and y
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether this is less than y
     */
    template<unsigned short size2>
    [[nodiscard]] bool operator<(const MyUint<size2>& y) const {
        //if the two numbers are equal return false
        if((*this) == y) return false;
        //if the raw binaries are the same size in bits then msb is equal to y.msb (in value)
        if ((size - msb) == (size2 - y.msb)){
            int i = msb, j = y.msb;
            //start going through the binaries
            for(; i < size && j < size2; i++,j++){
                //if both binary bits are on or off
                if(bits[i] && y.bits[j] || !bits[i] && !y.bits[j])
                    //continue the loop, this doesn't effect the relation
                    continue;

                //if the current bit in y is on while the bit in this is off
                if(!bits[i] && y.bits[j])
                    //y is larger than this (this is smaller than y)
                    return true;

                //if the current bit in y is off while the bit in this is on
                if(bits[i] && !y.bits[j])
                    //this is larger than y (y is smaller than this)
                    return false;
            }
        }
        //if not then one msb is larger than the other
        else
            // compare the raw location of the msb
            return (size - msb) < (size2 - y.msb);
    }
    /**
     * Greater than operator between this and y
     * To avoid code repetition I switch the places of this and y and compare them using <
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether y is less than this
     */
    template<unsigned short size2>
    [[nodiscard]] bool operator>(const MyUint<size2>& y) const { return y < (*this); }
    template<unsigned short size2>

    /**
     * Less than or equal to operator between this and y
     * Uses < and ==
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether this is less than or equal to y
     */
    [[nodiscard]] bool operator<=(const MyUint<size2>& y) const { return (*this) < y || (*this) == y; }

    /**
     * Greater than or equal to operator between this and y
     * Uses > and ==
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns whether this is greater than or equal to y
     */
    template<unsigned short size2>
    [[nodiscard]] bool operator>=(const MyUint<size2>& y) const { return (*this) > y || (*this) == y; }

    /*
     * Unsigned long long overloading
     *
     * In these functions a MyUint for the given unsigned long long x is created and compared to this as expected.
     */
    [[nodiscard]] bool operator==(const unsigned long long x) const {
        MyUint<size> y(x);
        return (*this) == y;
    }
    [[nodiscard]] bool operator!=(const unsigned long long x) const {
        MyUint<size> y(x);
        return (*this) != y;
    }
    [[nodiscard]] bool operator<(const unsigned long long x) const {
        MyUint<size> y(x);
        return y > (*this);
    }
    [[nodiscard]] bool operator>(const unsigned long long x) const {
        MyUint<size> y(x);
        return y < (*this);
    }
    [[nodiscard]] bool operator<=(const unsigned long long x) const {
        MyUint<size> y(x);
        return (*this) < y || (*this) == y; }
    [[nodiscard]] bool operator>=(const unsigned long long x) const {
        MyUint<size> y(x);
        return (*this) > y || (*this) == y;
    }
    /* ******************
     * Arithmetic operations
     */

    /**
     * Addition Assignment operator
     * Add y to this
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this after y has been added
     */
    template<unsigned short size2>
    MyUint& operator+=(const MyUint<size2>& y) {
        //define keep as  MyUint copy of this
        //perform bitwise and with y and then bit shift keep to the left
        MyUint<size> keep = (MyUint(*this) & y) << 1;
        //define res as a copy of this
        //perform bitwise xor with y
        MyUint<size> res = MyUint(*this) ^ y;
        //if keep is 0
        if (keep == 0){
            //set this to res and return
            *(this) = res;
            return *(this);
        }
        //add res to keep
        keep += res;
        //set this to res and return
        *(this) = keep;
        return *(this);
    }

    /**
     * Subtraction Assignment operator
     * Subtract y to this
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this after y has been subtracted
     */
    template<unsigned short size2>
    MyUint& operator-=(const MyUint<size2>& y) {
        //This algorithm gets weird undefined behaviour when y is greater than this, so I set this to 0 and return
        if ((*this) < y) {
            MyUint<size> z(0);
            *(this) = z;
            return *(this);
        }
        //copy y into y2. This has to be done with the vector<bool> constructor due to the difference in sizes
        MyUint<size> y2(y.bits);
        //define borrow as an empty number
        MyUint<size> borrow;
        //loop while y2 is not 0
        while (y2 != 0){
            //set borrow to not this
            //perform bitwise and on borrow
            borrow = (~*(this)) & y2;
            //xor this with y2
            (*this) ^= y2;
            //bitshift borrow to the left by one
            borrow <<= 1;
            //set y2 as borrow
            y2 = borrow;
        }
        return *(this);
    }

    //https://www.sciencedirect.com/topics/engineering/binary-multiplication
    /**
     * Multiplication Assignment operator
     * Multiply y to this
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this after y has been multiplied to it
     */
    template<unsigned short size2>
    MyUint& operator*=(const MyUint<size2>& y) {
        //define temp_number and result
        MyUint<size2> temp_number;
        MyUint<size> result(0);
        //start from the lsb, and loop until either this has been read fully or msb of y has been reached
        for(int i = size2 - 1;i >= 0 && i >= y.msb - 1; i--){
            //if the current bit in y is on
            if(y.bits[i]){
                //copy bits into temp_number via the vector<bool> constructor
                temp_number = MyUint<size2>(this->bits);
                //shift temp number by size2 - i - 1 to the left
                temp_number <<= (size2 - i - 1);
                //add temp_number to result
                result += temp_number;
            }
        }
        //set this equal to result
        *(this) = result;
        return *(this);
    }

    /**
     * Division Assignment operator
     * Divide y to this
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns this after y has been divided to it
     */
    template<unsigned short size2>
    MyUint& operator/=(const MyUint<size2>& y) {
        // Do not allow divisions by 0
        if(y == 0) throw std::runtime_error("Math error: Attempted to divide by Zero\n");
        // Quotient extracted so far
        MyUint<size> quotient(0);
        // intermediate remainder
        MyUint<size> rem(0);
        //loop though all the bits
        for(int i = 0; i < size; i++){
            //bitshift rem to the left
            rem <<= 1;
            //set the lsb in remainder to the current bit
            rem.bits[size - 1] = bits[i];
            //if rem is more than or equal to y
            if(rem >= y){
                //subtract y from rem
                rem -= y;
                //turn current bit in quotient on
                quotient.bits[i] = true;
            }
        }
        //verify msb of quotient and return it
        quotient.verify_msb();
        *(this) = quotient;
        return *(this);
    }
    /**
     * Modulo  Assignment operator
     * Divide y to this and
     * @tparam size2 number of bits of y
     * @param y second MyUint number
     * @return returns the remainder after y has been divided to this
     */
    template<unsigned short size2>
    MyUint& operator%=(const MyUint<size2>& y) {
        // Do not allow divisions by 0
        if(y == 0) throw std::runtime_error("Math error: Attempted to divide by Zero\n");
        // intermediate remainder
        MyUint<size> rem(0);
        //loop though all the bits
        for(int i = 0; i < size; i++){
            //bitshift rem to the left
            rem <<= 1;
            //set the lsb in remainder to the current bit
            rem.bits[size - 1] = bits[i];
            //if rem is more than or equal to y
            if(rem >= y){
                //subtract y from rem
                rem -= y;
            }
        }
        //verify msb of remainder and return it
        rem.verify_msb();
        *(this) = rem;
        return *(this);
    }

    // binary arithmetic operators
    // To avoid code duplication the respective arithmetic assignment operator is used on a copy of this and y
    template<unsigned short size2>
    [[nodiscard]] MyUint operator+(const MyUint<size2>& y) const{ return MyUint(*this) += y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator-(const MyUint<size2>& y) const { return MyUint(*this) -= y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator*(const MyUint<size2>& y) const { return MyUint(*this) *= y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator/(const MyUint<size2>& y) const { return MyUint(*this) /= y; }
    template<unsigned short size2>
    [[nodiscard]] MyUint operator%(const MyUint<size2>& y) const { return MyUint(*this) %= y; }

    /**
     * Postfix increment of a MyUint number
     * @return the number before increment
     */
    MyUint operator++(int){
        MyUint<size> old(*this);
        (*this) += 1;
        return old;
    }

    /**
    * Prefix increment of a MyUint number
    * @return the number after increment
    */
    MyUint operator++() {
        (*this) += 1;
        return (*this);
    }

    /**
     * Postfix decrement of a MyUint number
     * @return the number before decrement
     */
    MyUint operator--(int){
        MyUint<size> old(*this);
        (*this) -= 1;
        return old;
    }

    /**
    * Prefix decrement of a MyUint number
    * @return the number after decrement
    */
    MyUint operator--() {
        (*this) -= 1;
        return (*this);
    }

    /*
     * Unsigned long long overloading
     *
     * In these functions a MyUint for the given unsigned long long x is created and is operated on as expected by the
     * function
     */
    [[nodiscard]] MyUint operator+(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) += y;
    }
    MyUint& operator+=(const unsigned long long x) {
        MyUint<size> y(x);
        *this += y;
        return (*this);
    }
    [[nodiscard]] MyUint operator-(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) -= y;
    }
    MyUint& operator-=(const unsigned long long x) {
        MyUint<size> y(x);
        *this -= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator*(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) *= y;
    }
    MyUint& operator*=(const unsigned long long x) {
        MyUint<size> y(x);
        *this *= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator/(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) /= y;
    }
    MyUint& operator/=(const unsigned long long x) {
        MyUint<size> y(x);
        *this /= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator%(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) %= y;
    }
    MyUint& operator%=(const unsigned long long x) {
        MyUint<size> y(x);
        *this %= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator&(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) &= y;
    }
    MyUint& operator&=(const unsigned long long x) {
        MyUint<size> y(x);
        *this &= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator|(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) |= y;
    }
    MyUint& operator|=(const unsigned long long x) {
        MyUint<size> y(x);
        *this |= y;
        return (*this);
    }
    [[nodiscard]] MyUint operator^(const unsigned long long x) const {
        MyUint<size> y(x);
        return MyUint(*this) ^= y;
    }
    MyUint& operator^=(const unsigned long long x) {
        MyUint<size> y(x);
        *this ^= y;
        return (*this);
    }

    /**
     * Converts MyUint to either an int or long
     * @tparam type type to which the user wants MyUint to be converted to
     * @return type representing the value MyUint was representing or -1 if converstion is unsupported
     */
    template<typename type>
    [[nodiscard]] type convertTo(){
        //get size of type
        size_t s = sizeof(type);
        //if MyUint stores a number larger than what type can store throw an exception
        if(size - msb > sizeof(type) * 8) throw std::bad_cast();
        //convert MyUint to a binary string
        std::string binary = to_binary_string();
        //define y
        type y;
        //check size of type
        switch (s) {
            case 4:
                //type is integer
                //convert binary to int and return
                y = stoi(binary, nullptr, 2);
                break;
            case 8:
                //type is long
                //convert binary to long and return
                y = stol(binary, nullptr, 2);
                break;
            default:
                // -1 error
                y = -1;
        }
        return y;
    }

    /**
     * Convert MyUint to a binary string
     * @return Binary string representation of MyUint
     */
    [[nodiscard]] std::string to_binary_string() const {
        //define ret as an empty string
        std::string ret;
        //Loop through each bit
        for(auto bit: bits)
            //if bit is on append 1 else append 0
            bit ? ret += "1" : ret += "0";
        //if ret is empty return "0", else return the built string
        return ret.empty() ? "0" : ret;
    }

    [[nodiscard]] std::string to_string() const{
        std::string binary = this->to_binary_string();
        // Here we will store the resulting output
        std::string result{};

        // The conversion will also work for other  number bases
        // For size > 10 you need to adapt the creation of the digit at the bottom
        constexpr unsigned int numberBase{ 10 };

        // So, we will perform an integer division by 10, until the number is 0
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
            // Now "dividedNumberAsString" as string is the result of the dicvision by e.g. 10 in binary form
            binary = dividedNumberAsString;
            // The remainder is the number that we are interested in
            result.insert(0, 1, '0' + remainder);

            // Continue the loop with the new binary string
        } while (std::count(binary.begin(), binary.end(), '1'));

        // Show result
        return result.empty() ? "0" : result;
    }
};