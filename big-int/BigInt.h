//
// Created by a404m on 1/17/22.
//

#ifndef BIGINT_BIGINT_H
#define BIGINT_BIGINT_H

#include <vector>
#include <string>
#include <climits>
#include <compare>

class BigInt {
public:
    using Int = int32_t;
    using UInt = uint32_t;
    using BInt = int64_t;
    using UBInt = uint64_t;
private:
    static const UBInt MAX = UINT32_MAX;
    static const UBInt H_HALF = ~static_cast<UBInt>(MAX);
    static const UBInt L_HALF = MAX;

    bool sign = true;//0->- 1->+         used in compression
    std::vector<UInt> holder;
private:
    static UInt hHalf(UBInt);
    static UInt lHalf(UBInt);

    static std::string toBinary(UInt);

    void fit();

    bool isHolderGreater(const BigInt &) const;
    std::strong_ordering compareHolder(const BigInt &) const;

    void addToHolder(UBInt,decltype(holder.size()));
    void subToHolder(UInt,decltype(holder.size()));
public:
    BigInt(Int value = 0);
    explicit BigInt(const std::string &value);

    BigInt(const BigInt &value) = default;
    BigInt(BigInt &&value) noexcept;

    ~BigInt();

    BigInt &operator=(const BigInt &value) = default;
    BigInt &operator=(BigInt &&value) noexcept;

    BigInt operator+() const;
    BigInt operator-() const;

    BigInt &operator<<=(unsigned);
    BigInt &operator>>=(unsigned);

    BigInt &operator+=(BigInt);
    BigInt &operator-=(const BigInt&);

    BigInt &operator*=(BigInt);
    BigInt &operator/=(const BigInt&);
    BigInt &operator%=(const BigInt&);


    std::string toString() const;
    std::string toBinaryString() const;


    friend bool operator==(const BigInt &,const BigInt &);
    friend bool operator!=(const BigInt &,const BigInt &);
    friend bool operator>(const BigInt &,const BigInt &);
    friend bool operator<(const BigInt &,const BigInt &);
    friend bool operator>=(const BigInt &,const BigInt &);
    friend bool operator<=(const BigInt &,const BigInt &);

    friend std::strong_ordering operator<=>(const BigInt &,const BigInt &);

    friend BigInt operator+(BigInt ,const BigInt &);
    friend BigInt operator-(BigInt ,const BigInt &);

    friend BigInt operator*(BigInt ,const BigInt &);
    friend BigInt operator/(BigInt ,const BigInt &);
    friend BigInt operator%(BigInt ,const BigInt &);
};


#endif //BIGINT_BIGINT_H
