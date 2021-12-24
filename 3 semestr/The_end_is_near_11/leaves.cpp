#include "leaves.h"

using namespace Nature2;

Leaves::Leaves(std::string color, std::string draw){
    this->color = color;
    this->draw = draw;
}

void Leaves::Look(){
    std::cout << "Small leave with " << this->draw << " on it and beautiful " << this->color << " color" << std::endl;
}

Leaves::Leaves(){

}

Leaves::~Leaves(){

}


