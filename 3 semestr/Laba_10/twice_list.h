#ifndef TWICE_LIST_H
#define TWICE_LIST_H

template <class T>
class twice_list{
public:
    twice_list();
    ~twice_list();
private:
    T data;
    twice_list* left;
    twice_list* right;
};

#endif // TWICE_LIST_H
