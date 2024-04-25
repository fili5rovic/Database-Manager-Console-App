#ifndef PRVIPROJEKAT_CONDITION_H
#define PRVIPROJEKAT_CONDITION_H

#include "Record.h"

class Condition {
public:

    virtual bool evaluate(const Record& record) = 0;
private:

};


#endif //PRVIPROJEKAT_CONDITION_H
