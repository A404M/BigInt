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
    static const UInt HIGH_BIT = static_cast<UInt>(1)<<(sizeof(UInt)*8-1);

    bool sign = true;//0->- 1->+         used in compression
    std::vector<UInt> holder;
private:
    [[nodiscard]] static UInt hHalf(UBInt);
    [[nodiscard]] static UInt lHalf(UBInt);

    [[nodiscard]] static std::string toBinary(UInt);

    void fit();

    [[nodiscard]] bool isHolderGreater(const BigInt &) const;
    [[nodiscard]] std::strong_ordering compareHolder(const BigInt &) const;

    void addToHolder(UBInt,decltype(holder)::size_type);
    void subToHolder(UInt,decltype(holder)::size_type);
public:
    [[maybe_unused]] BigInt(Int value = 0);
    [[maybe_unused]] explicit BigInt(const std::string &value);

    [[maybe_unused]] BigInt(const BigInt &value) = default;
    [[maybe_unused]] BigInt(BigInt &&value) noexcept;

    ~BigInt();

    [[maybe_unused]] BigInt &operator=(const BigInt &value) = default;
    [[maybe_unused]] BigInt &operator=(BigInt &&value) noexcept;

    [[maybe_unused]][[nodiscard]] BigInt operator+() const;
    [[maybe_unused]][[nodiscard]] BigInt operator-() const;

    [[maybe_unused]] BigInt &operator<<=(unsigned);
    [[maybe_unused]] BigInt &operator>>=(unsigned);

    [[maybe_unused]] BigInt &operator+=(BigInt);
    [[maybe_unused]] BigInt &operator-=(const BigInt&);

    [[maybe_unused]] BigInt &operator*=(BigInt);
    [[maybe_unused]] BigInt &operator/=(const BigInt&);
    [[maybe_unused]] BigInt &operator%=(const BigInt&);


    [[maybe_unused]][[nodiscard]] std::string toString() const;
    [[maybe_unused]][[nodiscard]] std::string toBinaryString() const;


    template<typename T>
    [[maybe_unused]][[nodiscard]] T &to() {
        return *reinterpret_cast<T*>(&this->holder.front());
    }


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
