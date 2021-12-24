#ifndef PLANT_H
#define PLANT_H

#include <iostream>

namespace Nature{

class Plant
{
public:
    Plant();
    Plant(std::string color);
    ~Plant();

    void Look1();

protected:
    bool is_alive;
    std::string color;
};

}

#endif // PLANT_H
