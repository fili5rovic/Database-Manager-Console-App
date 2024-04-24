#ifndef PRVIPROJEKAT_ERRORS_H
#define PRVIPROJEKAT_ERRORS_H

#include <iostream>
using namespace std;

class EMultipleKeywordsException : public exception {
public:
    EMultipleKeywordsException(const string msg) : msg(msg) {}

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

class EMissingKeywordsException : public exception {
public:
    EMissingKeywordsException(const string msg) : msg(msg) {}

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

class EMissingArgumentsException : public exception {
public:
    EMissingArgumentsException(const string msg) : msg(msg) {}

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

class ENoKeywordsException : public exception {
public:
    ENoKeywordsException(const string msg) : msg(msg) {}

    virtual const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};




#endif //PRVIPROJEKAT_ERRORS_H
