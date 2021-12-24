#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H


class list_iterator{
public:
    list_iterator();
    ~list_iterator();
    int cbegin();
    int cend();
    void add();
    void erase();
};

#endif // LIST_ITERATOR_H
