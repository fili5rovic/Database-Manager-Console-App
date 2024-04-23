#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include "Statement.h"
#include "Filter.h"

class Select : Statement {
public:

    bool validate() override {


        return true;
    }
private:
    Filter filter;
};


#endif //PRVIPROJEKAT_SELECT_H
