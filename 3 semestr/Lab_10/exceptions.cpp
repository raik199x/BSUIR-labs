#include "exceptions.h"
#include <iostream>

using namespace std;

Exceptions::Exceptions(int code){
    this->code = code;
}

void Exceptions::what_happend(){
    switch (this->code) {
        case 0: cout << "0 exception" << endl; break;
        case 1: cout << "Cin error, possibly send wrong data" << endl; break;
        case 11: cout << "nullptr list" << endl; break;
        case 12: cout << "Out of range" << endl; break;
        default: break;
    }
}
