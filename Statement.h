#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"

class Statement {
public:

    virtual void init() = 0;



protected:
    Table* table;

    virtual bool validate(const string& str) = 0;

    Statement(Table* table) : table(table) {}
};


#endif //PRVIPROJEKAT_STATEMENT_H
