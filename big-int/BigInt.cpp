//
// Created by a404m on 1/17/22.
//

#include "BigInt.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>

BigInt::UInt BigInt::hHalf(UBInt var) {
    return var >> (sizeof(UInt) * 8);
}

BigInt::UInt BigInt::lHalf(UBInt var) {
    return static_cast<UInt>(var);
}

std::string BigInt::toBinary(UInt var) {
    std::string result;
    for(UInt i = HIGH_BIT;i != 0;i>>=1)
        result += (i&var)?'1':'0';
    return result;
}

void BigInt::fit(){
    for(auto it = this->holder.end()-1; it >= this->holder.begin(); --it){
        if(*it == 0)
            it = this->holder.erase(it);
        else
            break;
    }
    if(this->holder.empty())
        *this = BigInt(0);
    else
        this->holder.shrink_to_fit();
}

bool BigInt::isHolderGreater(const BigInt &b) const{
    if(this->holder.size() != b.holder.size()) {
        return this->holder.size() > b.holder.size();
    }else{
        auto ritb = b.holder.rbegin();
        for (auto rit = this->holder.rbegin(), rend = this->holder.rend()
                ; rit < rend; ++rit, ++ritb) {
            if(*rit != *ritb){
                return *rit > *ritb;
            }
        }
    }
    return false;
}

std::strong_ordering BigInt::compareHolder(const BigInt &b) const {
    if(this->holder.size() != b.holder.size()) {
        return this->holder.size() > b.holder.size()?
        std::strong_ordering::greater:
        std::strong_ordering::less;
    }else{
        auto ritb = b.holder.rbegin();
        for (auto rit = this->holder.rbegin(),
                rend = this->holder.rend()
                ; rit < rend; ++rit, ++ritb) {
            if(*rit != *ritb){
                return (*rit > *ritb)?
                       std::strong_ordering::greater:
                       std::strong_ordering::less;
            }
        }
    }
    return std::strong_ordering::equal;
}

void BigInt::addToHolder(UBInt val,decltype(holder)::size_type i){
    while(val){
        if(this->holder.size() > i){
            auto &ref = this->holder[i];
            val += ref;
            ref = lHalf(val);
        }else{
            this->holder.insert(this->holder.end(),i-this->holder.size()+1,0);
            this->holder.back() = lHalf(val);
        }
        val = hHalf(val);
        ++i;
    }
}
void BigInt::subToHolder(UInt v,decltype(holder.size()) i){
    UBInt val = v;
    if(this->holder.size() > i){
        auto &vI = this->holder[i];
        if(vI < val){//get carry
            auto j = i+1;
            auto &vJ = this->holder[j];
            while(true){//get one from the others
                if(j == this->holder.size())
                    throw std::logic_error("BigInt::subToHolder");
                else if(vJ != 0)
                    break;
                else
                    vJ = MAX;
                ++j;
            }
            --vJ;
            UBInt temp = static_cast<UBInt>(vI)+MAX+static_cast<UBInt>(1)-val;

            if(temp > MAX)
                throw std::logic_error("BigInt::subToHolder");

            vI = temp;
        }else{
            this->holder[i] -= val;
        }
    }else{
        throw std::logic_error("BigInt::subToHolder");
    }
    this->fit();
}

BigInt::BigInt(Int value) : sign(value >= 0), holder{value >= 0 ? static_cast<UInt>(value) : static_cast<UInt>(-value)} {
    //empty
}
BigInt::BigInt(const std::string &value) : BigInt(0){
    BigInt p = 1;
    bool s = true;
    auto rend = value.crend()-1;
    if(*rend == '-'){
        s = false;
    }else if(*rend == '+'){
        s = true;
    }else{
        ++rend;
    }
    for(auto rit = value.crbegin();rit < rend;++rit){
        auto digit = *rit-'0';
        if(digit < 0 || digit > 9)
            throw std::logic_error("BigInt::BigInt");
        *this += digit*p;
        p *= 10;
    }
    this->sign = s;
}
BigInt::BigInt(BigInt &&value) noexcept: sign(value.sign), holder(std::move(value.holder)) {
    //empty
}

BigInt &BigInt::operator=(BigInt &&value)  noexcept {
    std::swap(this->holder,value.holder);
    this->sign = value.sign;
    value.holder.clear();
    return *this;
}

BigInt BigInt::operator+() const{
    return *this;
}
BigInt BigInt::operator-() const{
    auto temp = *this;
    temp.sign = !temp.sign;
    return temp;
}

BigInt &BigInt::operator<<=(unsigned int n) {
    const int SIZE_BIT = sizeof(UInt)*8;
    const auto shift = n%SIZE_BIT;
    if(shift != 0) {
        this->holder.push_back(0);
        for (auto rit = this->holder.rbegin() + 1, rend = this->holder.rend(); rit < rend; ++rit) {
            UBInt temp = *rit;
            temp <<= shift;
            *(rit - 1) |= hHalf(temp);
            *rit = static_cast<UInt>(temp);
        }
        if(this->holder.back() == 0){//this does fit job
            this->holder.erase(this->holder.end()-1);
        }
    }
    this->holder.insert(this->holder.begin(),n/SIZE_BIT,0);
    //this->fit();
    return *this;
}

BigInt &BigInt::operator>>=(unsigned int n) {
    const int SIZE_BIT = sizeof(UInt)*8;
    const auto shift = n%SIZE_BIT;
    const auto eraseSize = n/SIZE_BIT;
    if(this->holder.size() > eraseSize) {
        this->holder.erase(this->holder.begin(),this->holder.begin()+eraseSize);
        if(shift != 0) {
            for (auto it = this->holder.begin(), end = this->holder.end(); it < end; ++it) {
                UBInt temp = *it;
                temp <<= sizeof(UInt) * 8 - shift;
                if(it != this->holder.begin())
                    *(it - 1) |= lHalf(temp);
                *it = hHalf(temp);
            }
        }
    }else{
        *this = 0;
    }

    this->fit();
    return *this;
}

BigInt &BigInt::operator+=(BigInt value) {
    if(*this == 0){
        std::swap(*this,value);
    }else if(this->sign == value.sign){
        for(decltype(value.holder.size()) i = 0;i < value.holder.size();++i){
            addToHolder(value.holder[i],i);
        }
    }else{
        if(value.isHolderGreater(*this)){
            std::swap(*this,value);
        }
        for(decltype(value.holder.size()) i = 0;i < value.holder.size();++i){
            subToHolder(value.holder[i],i);
        }
    }

    return *this;
}

BigInt &BigInt::operator-=(const BigInt &value) {
    if(this == &value){
        *this = 0;
        return *this;
    }
    *this += -value;
    return *this;
}

BigInt &BigInt::operator*=(BigInt value) {
    BigInt temp = 0;
    std::swap(*this,temp);
    this->sign = temp.sign == value.sign;//assign the sign

    auto ts = temp.holder.size(),vs = value.holder.size();

    for(decltype(this->holder)::size_type i = 0;i < vs;++i){
        UBInt t = value.holder[i];
        if(t == 0)
            continue;

        for(decltype(i) j = 0;j < ts;++j){
            this->addToHolder(t*temp.holder[j],i+j);
        }
    }
    this->fit();
    return *this;
}
BigInt &BigInt::operator*=(UInt val) {
    BigInt temp = 0;
    std::swap(*this,temp);
    auto ts = temp.holder.size();
    this->sign = temp.sign;

    for(decltype(ts) j = 0;j < ts;++j){
        this->addToHolder(static_cast<UBInt>(val)*temp.holder[j],j);
    }

    return *this;
}

#define SHIFT_TO_RIGHT(add,index,HIGH_BIT) \
if((add) == 1){         \
    --(index);          \
    if((index) < 0){    \
        goto RET;       \
    }                   \
    (add) = (HIGH_BIT); \
}else{                  \
    (add) >>= 1;        \
}

#define SHIFT_TO_LEFT(add,index,HIGH_BIT) \
if((add) == (HIGH_BIT)){    \
    ++(index);              \
    (add) = 1;              \
}else{                      \
    (add) <<= 1;            \
}

BigInt &BigInt::operator/=(const BigInt &value) {
    if(value == 0){
        throw std::logic_error("Dividing by zero");
    }

    if(&value == this){
        *this = 1;
        return *this;
    }else if(value.isHolderGreater(*this)){
        *this = 0;
        return *this;
    }

    auto tb = value;
    UInt add = 1;
    int index = 0;

    BigInt temp = 0;
    std::swap(*this,temp);
    this->sign = temp.sign == value.sign;//assign the sign

    tb.sign = false;//negative
    temp.sign = true;//positive

    while(true){
        auto test = temp.compareHolder(tb);
        if(test == std::strong_ordering::greater){
            decltype(index) diff = temp.holder.size()-tb.holder.size();
            if(diff) {
                index += diff;
                tb.holder.insert(tb.holder.begin(), diff, 0);
                continue;
            }
            auto q = temp.holder.back()/tb.holder.back();
            int l2 = 0;
            while(true){
                q >>= 1;
                if(!q){
                    break;
                }
                ++l2;
                SHIFT_TO_LEFT(add,index,HIGH_BIT)
            }
            if(l2){
                tb <<= l2;
                if(tb.isHolderGreater(temp)) {
                    tb >>= 1;
                    if(add == 1){
                        --index;
                        add = HIGH_BIT;
                    }else{
                        add >>= 1;
                    }
                }
            }
        }else if(test == std::strong_ordering::less){
            decltype(index) diff = tb.holder.size()-temp.holder.size();
            UBInt q;
            if(diff) {
                if(diff > index){
                    q = *reinterpret_cast<UBInt*>(&(tb.holder.back()))/temp.holder.back();
                    goto REST_IN_LESS;
                }
                index -= diff;
                if(index < 0){
                    goto RET;
                }else{
                    tb.holder.erase(tb.holder.begin(),tb.holder.begin()+diff);
                    continue;
                }
            }
            q = tb.holder.back()/temp.holder.back();
            REST_IN_LESS:
            int l2 = 0;
            while(q){
                SHIFT_TO_RIGHT(add,index,HIGH_BIT)
                q >>= 1;
                ++l2;
            }
            if(l2){
                tb >>= l2;
            }
        }else{//equal
            this->addToHolder(add,index);
            break;
        }
        temp += tb;//tb is negative
        this->addToHolder(add,index);
    }

    RET:
    return *this;
}
BigInt &BigInt::operator/=(UInt val) {
    BigInt temp = 0;
    std::swap(*this,temp);
    this->sign = temp.sign;
    auto rit = temp.holder.rbegin();
    UBInt q = *rit/val;
    UBInt r = *rit%val;
    ++rit;
    this->holder.front() = q;
    for(auto rend = temp.holder.rend();rit != rend;++rit){
        UBInt t = r;
        t <<= sizeof(UInt)*8;
        t |= *rit;
        q = t/val;
        r = t%val;
        this->holder.insert(this->holder.begin(),0);
        this->holder.front() = q;
    }
    this->fit();
    return *this;
}

BigInt &BigInt::operator%=(const BigInt &value) {
    if(value == 0){
        throw std::logic_error("Dividing by zero");
    }
    if(&value == this){
        *this = 0;
        return *this;
    }

    auto tb = value;

    if(tb.holder.size() < this->holder.size()){
        auto diff = this->holder.size()-tb.holder.size();
        tb.holder.insert(tb.holder.begin(),diff,0);
    }

    auto tSign = this->sign == value.sign;

    tb.sign = false;
    this->sign = true;

    while(true){
        auto test = this->compareHolder(tb);
        if(test == std::strong_ordering::greater){
            auto diff = this->holder.size()-tb.holder.size();
            if(diff){
                tb.holder.insert(tb.holder.begin(),diff,0);
                continue;
            }
            tb *= this->holder.back()/tb.holder.back();
        }else if(test == std::strong_ordering::less){
            auto diff = tb.holder.size()-this->holder.size();
            if(diff){
                tb.holder.erase(tb.holder.begin(),tb.holder.begin()+diff);
                if(value.isHolderGreater(tb)){
                    goto RET;
                }
                continue;
            }
            ///todo
        }else{//equal
            *this = 0;
            return *this;
        }
        END:
        *this += tb;
    }

    RET:
    tb = value;
    tb.sign = true;
    if(!tSign){//valSign == -
        *this = tb-*this;
    }
    this->sign = value.sign;
    return *this;
}

std::string BigInt::toString() const {
    if(*this == 0)
        return "0";
    std::string result = (this->sign)?"":"-";
    auto temp = *this;
    temp.sign = true;
    while(temp != 0){
        result += (temp%10).holder.front()+'0';
        temp /= 10;
    }
    std::reverse(result.begin(),result.end());
    return result;
}
std::string BigInt::toBinaryString() const {
    if(*this == 0){
        return "0";
    }
    std::string result;
    for(auto rit = this->holder.rbegin(),rend = this->holder.rend();rit < rend;++rit){
        result += toBinary(*rit);
    }
    for(auto it = result.begin();it < result.end();){
        if(*it == '0'){
            it = result.erase(it);
        }else{
            break;
        }
    }
    if(!this->sign){
        result = '-'+result;
    }
    return result;
}



//friends

bool operator==(const BigInt &b1,const BigInt &b2){
    return b1.sign == b2.sign && b1.holder.size() == b2.holder.size() && b1.holder == b2.holder;
}
bool operator!=(const BigInt &b1,const BigInt &b2){
    return b1.sign != b2.sign || b1.holder.size() != b2.holder.size() || b1.holder != b2.holder;
}
bool operator>(const BigInt &b1,const BigInt &b2){
    if(b1.sign != b2.sign){
        return b1.sign > b2.sign;
    }else if(b1.sign){
        return b1.isHolderGreater(b2);
    }else{
        return b2.isHolderGreater(b1);
    }
}
bool operator<(const BigInt &b1,const BigInt &b2){
    if(b1.sign != b2.sign){
        return b1.sign < b2.sign;
    }else if(b1.sign){
        return b2.isHolderGreater(b1);
    }else{
        return b1.isHolderGreater(b2);
    }
}
bool operator>=(const BigInt &b1,const BigInt &b2){
    return !(b1 < b2);
}
bool operator<=(const BigInt &b1,const BigInt &b2){
    return !(b1 > b2);
}

std::strong_ordering operator<=>(const BigInt &b1, const BigInt &b2) {
    if(b1.sign != b2.sign){
        return (b1 > b2)?std::strong_ordering::greater : std::strong_ordering::less;
    }else if(b1.sign){
        return b1.compareHolder(b2);
    }else{
        return b2.compareHolder(b1);
    }
}

BigInt operator+(BigInt b1, const BigInt &b2) {
    return b1 += b2;
}
BigInt operator-(BigInt b1, const BigInt &b2) {
    return b1 -= b2;
}

BigInt operator*(BigInt b1, const BigInt &b2) {
    return b1 *= b2;
}
BigInt operator/(BigInt b1, const BigInt &b2) {
    return b1 /= b2;
}
BigInt operator%(BigInt b1, const BigInt &b2) {
    return b1 %= b2;
}
