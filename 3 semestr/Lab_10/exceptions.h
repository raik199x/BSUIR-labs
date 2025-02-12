#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


class Exceptions{
public:
    Exceptions(int code);
    void what_happened();
private:
    int code;
};

#endif // EXCEPTIONS_H
