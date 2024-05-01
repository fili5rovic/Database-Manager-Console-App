#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"

class Statement {
public:

    virtual void execute() = 0;



protected:
    Table* table;
    const string inputQuery;


    Statement(Table* table, const string& inputQuery) : table(table), inputQuery(inputQuery) {}
};


#endif //PRVIPROJEKAT_STATEMENT_H
