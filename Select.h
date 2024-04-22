#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include "Statement.h"
#include "Filter.h"

class Select : Statement {
public:

private:
    Filter& filter;
};


#endif //PRVIPROJEKAT_SELECT_H
