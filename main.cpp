#include <iostream>
#include "big-int/BigInt.h"

#define TEST

#ifdef TEST
#include <gmpxx.h>
#endif

using namespace std;
#ifdef TEST
string bin(mpz_class a){
    string str;
    while(a != 0){
        str += (a%2==1)?"1":"0";
        a/=2;
    }
    std::reverse(str.begin(), str.end());
    return str;
}
#endif

int main() {
    string num1 = "-9";
    string num2 = "-4";
#ifdef TEST
    mpz_class a{num1};
    mpz_class b{num2};
    a %= b;
    string binA = bin(a);
    cout << (a<0?"-":"")+binA << endl;
#endif
    BigInt b1{num1};
    BigInt b2{num2};
    b1 %= b2;
    string binB = b1.toBinaryString();
    cout << binB << endl;
#ifdef TEST
    cout << (binB == binA);
#endif
    return 0;
}
