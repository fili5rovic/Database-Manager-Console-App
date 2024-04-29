#ifndef PRVIPROJEKAT_CONDITION_H
#define PRVIPROJEKAT_CONDITION_H

#include <memory>
#include "Record.h"
#include "Table.h"

class Condition {
public:
    Condition(const Table* table, const string& columnName, const string& value) : table(table), value(value) {
        initFieldPositionInVector(columnName);
    }

    shared_ptr<Table> getConditionedTable() const {
        shared_ptr<Table> nTable = std::make_shared<Table>(table->getName());

        for(const string& columnName : table->getTableHeaders()) {
            nTable->addHeader(columnName);
        }

        for(const Record& record : table->getTableRecords()) {
            if(evaluate(record))
                nTable->addRecord(record);
        }
        return nTable;
    }

    const vector<Record> getConditionedRows() const {
        vector<Record> records;
        for(const Record& currRecord : table->getTableRecords()) {
            if(evaluate(currRecord))
                records.push_back(currRecord);
        }
        return records;
    }

    virtual bool evaluate(const Record& record) const = 0;
protected:
    const Table* table;
    int fieldPositionInVector;
    string value;

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



};


#endif //PRVIPROJEKAT_CONDITION_H
