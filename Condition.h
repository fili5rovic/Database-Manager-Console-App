#ifndef PRVIPROJEKAT_CONDITION_H
#define PRVIPROJEKAT_CONDITION_H

#include <memory>
#include <regex>
#include "Record.h"
#include "Table.h"
#include "Errors.h"

class Condition {
public:
    Condition(const Table* table, const string& columnName, const string& value) : table(table), value(value) {
        initFieldPositionInVector(columnName);
    }

    Table* getConditionedTable() const {
        Table* nTable = new Table(table->getName());

        for(const string& columnName : table->getTableHeaders()) {
            nTable->addHeader(columnName);
        }

        for(Record& record : table->getTableRecordsByReference()) {
            if(evaluate(record))
                nTable->addRecord(record);
        }
        return nTable;
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
            if(regex_match(table->getTableHeaders().at(i), regex(columnName, regex_constants::icase))) {
                fieldPositionInVector = i;
                break;
            }
        }
        if(fieldPositionInVector == -1) {
            throw EInvalidColumnNameException("[ERROR] Column name '" + columnName + "' does not exist inside table " + table->getName());
            cout << "INVALID COLUMN NAME" << endl;
            fieldPositionInVector = 0;
        }
    }



};


#endif //PRVIPROJEKAT_CONDITION_H
