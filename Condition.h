#ifndef PRVIPROJEKAT_CONDITION_H
#define PRVIPROJEKAT_CONDITION_H

#include "Record.h"
#include "Table.h"

class Condition {
public:
    Condition(Table* table, const string& columnName, const string& value) : table(table), value(value) {
        initFieldPositionInVector(columnName);
    }

    Table* getConditionedTable() const {
        Table* nTable = new Table(table->getName());

        for(const string& columnName : table->getTableHeaders()) {
            nTable->addHeader(columnName);
        }

        for(const Record& record : table->getTableRecords()) {
            if(record.getData().at(fieldPositionInVector) == value)
                nTable->addRecord(record);
        }
        return nTable;
    }
private:
    void initFieldPositionInVector(const string& columnName) {
        fieldPositionInVector = -1;
        for(int i = 0; i < table->getTableHeaders().size(); i++) {
            if(table->getTableHeaders().at(i) == columnName) {
                fieldPositionInVector = i;
                break;
            }
        }
        if(fieldPositionInVector == -1) {
            cout << "INVALID COLUMN NAME" << endl;
        }
    }


    Table* table;
    int fieldPositionInVector;
    string value;
};


#endif //PRVIPROJEKAT_CONDITION_H
