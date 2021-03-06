#include <iostream>
#include "big-int/BigInt.h"
#include <chrono>

#define TEST
//#define TEST2

#ifdef TEST
#include <gmpxx.h>
#endif
#ifdef TEST2
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;
#endif

using namespace std;

#ifdef TEST
template<typename T>
string bin(T a){
    string str;
    do{
        str += (a%2==1)?"1":"0";
        a/=2;
    }while(a != 0);
    std::reverse(str.begin(), str.end());
    return str;
}
#endif

#define OPERATION /=

int count = 0;

int main() {
    string num1 = "4234214767846847618713214064372861784561374068712364897442342147678468476187345413490651654684987354613211046120657484634513215464613847616407136798463453267498464201059709870485841640643728617845613740687123648974";
    string num2 = "543789256939256934785623784672394780143748165817047103865693478562378658170471038656934785623784672394780143748165817047103865168654103641023647";
    //string num1 = "126548878";
    decltype(chrono::high_resolution_clock::now()) start;
#ifdef TEST
    mpz_class a{num1};
    mpz_class b{num2};
    start = chrono::high_resolution_clock::now();
    a OPERATION b;
    cout << chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count() << "ms\n";
    string ra = bin(a);
    cout << ra << endl;
#endif
#ifdef TEST2
    cpp_int ci1{num1};
    cpp_int ci2{num2};
    start = chrono::high_resolution_clock::now();
    ci1 OPERATION ci2;
    cout << chrono::duration<double,milli>(chrono::high_resolution_clock::now() - start).count() << "ms\n";
    string rci = ci1.str();
    cout << rci << endl;
#endif
    BigInt b1{num1};
    BigInt b2{num2};
    start = chrono::high_resolution_clock::now();
    b1 OPERATION b2;
    cout << chrono::duration<double, milli>(chrono::high_resolution_clock::now() - start).count() << "ms\n";
    string rb = b1.toBinaryString();
    cout << rb << endl;
#ifdef TEST
    cout << (rb == ra);
#endif
#ifdef TEST2
    cout << (rb == rci);
#endif
    cout << endl << endl;
    return 0;
}
