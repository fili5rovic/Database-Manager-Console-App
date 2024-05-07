#ifndef PRVIPROJEKAT_INSERT_H
#define PRVIPROJEKAT_INSERT_H

#include "Statement.h"
#include "Database.h"

class Insert : public Statement {
public:
    Insert(const string &input, shared_ptr<Database> database) : Statement(input, database) {}

private:
    void executingQuery(const smatch &matches) const override {
        string tableName = matches[1];
        vector<string> listOfColumns = StringManipulator::instance().splitString(matches[2], ',');
        vector<string> values = StringManipulator::instance().splitString(matches[3], ',');

        checkIfAllColumnsAreThere(listOfColumns, values.size());

        sortColumnsAndValues(listOfColumns, values);

        StringManipulator::instance().removeQuotesAndLeadingSpaces(values);

        Record record = Record();
        for (const string &val: values) {
            record.addData(val);
        }

        table->addRecord(record);

        cout << *table;
    }

    void sortColumnsAndValues(vector<string> &listOfColumns, vector<string> &values) const {
        StringManipulator::instance().removeQuotesAndLeadingSpaces(listOfColumns);
        vector<string> oldValues = values;
        for(int i = 0; i < table->getTableHeaders().size(); i++) {
            for (int j = 0; j < listOfColumns.size(); ++j) {
                cout << "i: " << table->getTableHeaders()[i] << " j: " << listOfColumns[j] << endl;
                if(regex_match(table->getTableHeaders()[i], regex(listOfColumns[j], regex_constants::icase))) {
                    values.at(i) = oldValues.at(j);
                    break;
                }
            }
        }
        listOfColumns = table->getTableHeaders();

        cout << "Columns: ";
        for (const string &column: listOfColumns) {
            cout << column << " ";
        }
        cout << endl;
        cout << "Values: ";
        for (const string &value: values) {
            cout << value << " ";
        }
        cout << endl;
    }

    // todo doesnt work if i don't write all of columns
    void checkIfAllColumnsAreThere(vector<string> listOfColumns, const size_t listOfValuesSize) const {
        if (listOfColumns.size() > table->getTableHeaders().size())
            throw EBadArgumentsException(
                    "[RUNTIME_ERROR] The number of selected columns exceeds the column count in table '" + table->getName() + "'.");
        else if (listOfColumns.size() < table->getTableHeaders().size())
            throw EBadArgumentsException(
                    "[RUNTIME_ERROR] The number of selected columns is less than the column count in table '" + table->getName() + "'.");

        if (listOfValuesSize < listOfColumns.size())
            throw EBadArgumentsException(
                    "[RUNTIME_ERROR] Every column from " + table->getName() + " should be given value.");
        else if (listOfValuesSize > listOfColumns.size())
            throw EBadArgumentsException("[RUNTIME_ERROR] Too many values for columns in table " + table->getName() + ".");

        bool found = false;
        for (string &column: listOfColumns) {
            for (const string &tableColumn: table->getTableHeaders()) {
                if (regex_match(column, regex("\\s*(?:\\'|\\\")\\s*" + tableColumn + "\\s*(?:\\'|\\\")\\s*",
                                              regex_constants::icase))) {
                    found = true;
                    break;
                }
            }
            if (!found)
                throw EBadArgumentsException("[RUNTIME_ERROR] " + column + " does not exist in " + table->getName());
        }
    }


    //<editor-fold desc="Error Handling">
    void runtimeErrors(const string &input) const override {
        cout << "DIDNT MATCH" << endl;
        if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
        }

        if (regex_match(input,
                        regex("^\\s*insert\\s+into\\s+([a-zA-Z_]+)\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\"|\\w+)\\s*(?:\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\"|\\w+)\\s*)*)\\s*\\)\\s+values\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\"|\\w+)\\s*(?:\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\"|\\w+))*)*\\s*\\)\\s*",
                              regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Missing \" around arguments.");
        }
        throw EBadArgumentsException("[RUNTIME_ERROR] Table insert error.");
    }


    void checkForSyntaxErrors(const string &query) const {
        if (!regex_match(query, regex(".*\\s+into.*", regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No INTO keyword");
        if (!regex_match(query, regex(".*\\s+values.*", regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No VALUES keyword");
        if (regex_match(query, regex(".*\\s+into\\s*$", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] INSERT INTO has no argument.");
        }

        // multiple keywords
        if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bselect\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bcreate\\b.*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple INSERT keywords not allowed within the same statement.");
        }
    }
    //</editor-fold>


    //<editor-fold desc="Getters">
    regex getRegexPattern() const override {
        return regex(
                "^\\s*insert\\s+into\\s+([a-zA-Z_]+)\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\")\\s*(?:\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\")\\s*)*)\\s*\\)\\s+values\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\")\\s*(?:\\s*\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\"))*)*\\s*\\)\\s*$",
                regex_constants::icase);
    }


    regex getRegexForFindingTable() const override {
        return regex("into\\s+(\\w+)",regex_constants::icase);
    }
    //</editor-fold>

};


#endif //PRVIPROJEKAT_INSERT_H
