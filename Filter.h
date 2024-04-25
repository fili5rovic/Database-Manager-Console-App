#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include "Condition.h"

class Filter {
public:
    Filter(const vector<Condition*>& cond) : conditions(cond) {}

    vector<Record> applyFilter(const vector<Record>& records) const {
        return records;
    }

private:
    vector<Condition*> conditions;
};


#endif //PRVIPROJEKAT_FILTER_H
