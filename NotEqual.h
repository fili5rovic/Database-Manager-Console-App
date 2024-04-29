#ifndef PRVIPROJEKAT_NOTEQUAL_H
#define PRVIPROJEKAT_NOTEQUAL_H

#include "Condition.h"

class NotEqual : public Condition {
public:
    NotEqual(const Table* table, const string &columnName, const string &value)  : Condition(table, columnName, value) {}

    bool evaluate(const Record &record) const override {
        return record.getData().at(this->fieldPositionInVector) != this->value;
    }
private:
};


#endif //PRVIPROJEKAT_NOTEQUAL_H
