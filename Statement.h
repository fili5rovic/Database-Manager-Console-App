#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"

class Statement {
public:

    virtual bool validate() = 0;
private:
    Table* t;
};


#endif //PRVIPROJEKAT_STATEMENT_H
