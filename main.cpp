#include <iostream>
#include "big-int/BigInt.h"
#include <gmpxx.h>

using namespace std;

string bin(mpz_class a){
    string str;
    while(a != 0){
        str += (a%2==1)?"1":"0";
        a/=2;
    }
    std::reverse(str.begin(), str.end());
    return str;
}

int main() {
    string num1 = "100012354564684351321654684687487979841321654154542528457928435637846124600000000000";
    string num2 = "1287948965746546848746387446545681236389572756";
    mpz_class a{num1};
    mpz_class b{num2};
    a /= b;
    cout << "0b" + bin(a) << endl;
    BigInt b1{num1};
    BigInt b2{num2};
    b1 /= b2;
    cout << "0b" + b1.toBinaryString();
    return 0;
}
