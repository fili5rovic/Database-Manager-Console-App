#ifndef PRVIPROJEKAT_EQUAL_H
#define PRVIPROJEKAT_EQUAL_H


#include "Condition.h"

class Equal : public Condition {
public:
    Equal(const Table *table, const string &columnName, const string &value)  : Condition(table, columnName, value) {}

    bool evaluate(const Record &record) const override {
        return record.getData().at(this->fieldPositionInVector) == this->value;
    }

private:
};


#endif //PRVIPROJEKAT_EQUAL_H
