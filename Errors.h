#ifndef PRVIPROJEKAT_ERRORS_H
#define PRVIPROJEKAT_ERRORS_H

#include <iostream>
using namespace std;

class MyException : public exception {
public:
    MyException(const string msg) : msg(msg) {}

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

class EMultipleKeywordsException : public MyException {
public:
    EMultipleKeywordsException(const string msg) : MyException(msg) {}
};

class EMissingKeywordsException : public MyException {
public:
    EMissingKeywordsException(const string msg) : MyException(msg) {}
};

class EMissingArgumentsException : public MyException {
public:
    EMissingArgumentsException(const string msg) : MyException(msg) {}
};

class EBadArgumentsException : public MyException {
public:
    EBadArgumentsException(const string msg) : MyException(msg) {}
};

class ENoKeywordsException : public MyException {
public:
    ENoKeywordsException(const string msg) : MyException(msg) {}
};


class EInvalidColumnNameException : public MyException {
public:
    EInvalidColumnNameException(const string msg) : MyException(msg) {}
};


class EFileNotOpen : public MyException {
public:
    EFileNotOpen(const string msg) : MyException(msg) {}
};


#endif //PRVIPROJEKAT_ERRORS_H
