#ifndef TREE_H
#define TREE_H

#include "plant.h"
#include "leaves.h"

#include <iostream>

namespace Nature{

class Tree:public Plant{
    friend class Neste;
public:
    Tree();
    Tree(std::string type, int height, std::string color);
    ~Tree();
    void Look();
        class Neste{
        friend class Tree;
        public:
            Neste();
            Neste(int population,int capacity);
            void check_neste();
            ~Neste();
        private:
            int population;
            int capacity;
        };

private:
    std::string type;
    int height;
    Neste *something;
};

}
#endif // TREE_H
