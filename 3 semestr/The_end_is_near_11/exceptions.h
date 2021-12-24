#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H



class Exceptions
{
public:
    Exceptions(int code);
    ~Exceptions();
    void what_happend();
private:
    int code;
};


#endif // EXCEPTIONS_H
