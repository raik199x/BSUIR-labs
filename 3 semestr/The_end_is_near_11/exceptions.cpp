#include "exceptions.h"
#include <iostream>


Exceptions::Exceptions(int code){
    this->code = code;
}

Exceptions::~Exceptions(){

}

void Exceptions::what_happend(){
    std::cin.clear();
    switch (this->code) {
    case 1: std::cout << "Cin failed!" << std::endl; std::cin.ignore(100000,'\n'); return;
    case 2: std::cout << "0 exception in another namespace" << std::endl; return;
    default: std::cout << "Unexpected!" << std::endl; return;
    }
}
