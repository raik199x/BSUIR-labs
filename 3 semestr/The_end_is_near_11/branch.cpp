#include "branch.h"

using namespace Nature;

Branch::Branch(int size)
{
    this->size = size;
    this->durability = 100;
    this->something_on_branch = nullptr;
    this->leave = nullptr;
}

Nature2::Leaves Branch::operator=(Nature2::Leaves &other){
    if(this->leave != nullptr)
        delete this->leave;
    this->leave = new Nature2::Leaves;
    this->leave->color = other.color;
    this->leave->draw = other.draw;
    return *(this->leave);
}

Branch::~Branch(){
    if(this->leave != nullptr)
        delete this->leave;
}

Branch::Fruit::~Fruit(){

}

void Branch::create_fruite(bool is_eateble, std::string name){
    if(this->something_on_branch != nullptr)
        delete this->something_on_branch;
    this->something_on_branch = new Fruit(is_eateble,name);
}

void Branch::Look(){
    if(this->durability == 0){
        std::cout << "This branch is broken =(" << std::endl;
        if(something_on_branch != nullptr){
            std::cout << "There used to be ";
            this->something_on_branch->what_is_it();
        }
        return;
    }
    std::cout << "The size of this branch is - " << this->size << std::endl;
    if(size < 10)
        std::cout << "Looks kinda sma... adequatly sized, yap" << std::endl;
    else if (size < 20)
        std::cout << "Just Normal branch, what can i say" << std::endl;
    else if (size > 20)
        std::cout << "DAMN it's huge... does branch with this size even exist?" << std::endl;
    if (this->something_on_branch != nullptr)
        this->something_on_branch->what_is_it();
    if (this->leave != nullptr)
        this->leave->Look();
}

void Branch::try_push(){
    switch (this->durability) {
    case 100: std::cout << "Such a strong branch wount break... sure?" << std::endl; this->durability-=30; break;
    case 70:  std::cout << "This tree is goona have BAD TIME" << std::endl; this->durability-=30; break;
    case 40:  std::cout << "Wh...What the point?" << std::endl; this->durability-=30; break;
    case 10: std::cout << "You are a terrible person!" << std::endl; this->durability-=10; break;
    case 0: std::cout << "This branch is broken, nothing to do with it" << std::endl; break;
    }
}

Branch::Fruit::Fruit(bool is_eateble, std::string name){
    this->is_eateble = is_eateble;
    this->name = name;
}

void Branch::Fruit::what_is_it(){
    std::cout << this->name;
    if(this->is_eateble == true)
        std::cout << "It should be tasty enough" << std::endl;
    else
        std::cout << "Probably shouldnt eat it" << std::endl;
}
