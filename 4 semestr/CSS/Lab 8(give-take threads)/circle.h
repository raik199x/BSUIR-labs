#pragma once
#include <stdlib.h>

#include <sys/types.h>



struct Circle{
    char message[255];
    unsigned long long checksum;
    unsigned int added;
    unsigned int deleted;
    struct Circle *left;
    struct Circle *right;
};

void AppendCircle(struct Circle **to_append);