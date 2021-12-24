#ifndef LEAVES_H
#define LEAVES_H

#include <iostream>

namespace Nature2{

class Leaves
{
public:
    Leaves();
    Leaves(std::string color, std::string draw);
    ~Leaves();

    void Look();
    std::string color;
    std::string draw;
};

}

#endif // LEAVES_H
