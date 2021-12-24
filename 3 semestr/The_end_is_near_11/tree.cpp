#include "tree.h"

using namespace Nature;

Tree::Tree(std::string type, int size, std::string color)
{
    srand(time(0));
    this->type = type;
    this->height = size;
    this->something = new Neste;
    this->something->capacity = rand() % 10;
    this->something->population = rand() % 10;
    this->is_alive = true;
    this->color = color;
}

Tree::~Tree(){
    if(this->something != nullptr)
        delete this->something;
}

Tree::Neste::Neste(){

}

Tree::Neste::Neste(int pop, int cap){
    this->population = pop;
    this->capacity = cap;
}

Tree::Neste::~Neste(){

}

void Tree::Neste::check_neste(){
    std::cout << this->capacity << " birds could live here" << std::endl << "Currently live: " << this->population << std::endl;
    if (this->population == this->capacity)
        std::cout << "It's actually full" << std::endl;
    else if (this->population > this->capacity)
        std::cout << "Just... how?" << std::endl;
    else if (this->population == 0)
        std::cout << "Sad to see it empty" << std::endl;
    else
        std::cout << "Life is growing here" << std::endl;
}



void Tree::Look(){
    std::cout << "This is a " << this->type << " tree" << std::endl;
    std::cout << "Also it's really huge, like " << this->height << "m long" << std::endl;
    std::cout << "Beautiful " << this->color << " color of crown" << std::endl;
        std::cout << std::endl << "Look, there is a neste here!" << std::endl;
        this->something->check_neste();
}
