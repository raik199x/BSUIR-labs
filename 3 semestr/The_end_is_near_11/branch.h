#ifndef BRANCH_H
#define BRANCH_H
#include <iostream>
#include "leaves.h"

namespace Nature{

class Branch{
   friend class Fruit;
public:
   Branch();
    Branch(int size);
    ~Branch();

    void Look();
    void try_push();
    void create_fruite(bool is_eateble, std::string name);
    class Fruit{
    public:
        Fruit();
        Fruit(bool is_eateble, std::string name);
        void what_is_it();
        ~Fruit();
    private:
        std::string name;
        bool is_eateble;
    };
    Nature2::Leaves operator=(Nature2::Leaves &other);
private:
    int size;
    int durability;
    Fruit *something_on_branch;
    Nature2::Leaves *leave;
};
}
#endif // BRANCH_H
