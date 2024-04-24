#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"

class Statement {
public:

    virtual void init() = 0;



private:
    Table* t;

    virtual bool validate(const string& str) = 0;
};


#endif //PRVIPROJEKAT_STATEMENT_H
