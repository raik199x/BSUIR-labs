#include "plant.h"

using namespace Nature;

Plant::Plant()
{
    this->is_alive = true;
}

Plant::Plant(std::string color):Plant(){
    this->color = color;
}

void Plant::Look1(){
    if(this->is_alive == true)
        std::cout << "The beauty of nature" << std::endl << "You can see the color of it, it's " << this->color << std::endl;
    else
       std::cout << "Dead one, so sad to see...." << std::endl;

}

Plant::~Plant(){

}
