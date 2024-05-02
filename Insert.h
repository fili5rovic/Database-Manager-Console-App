#ifndef PRVIPROJEKAT_INSERT_H
#define PRVIPROJEKAT_INSERT_H

#include "Statement.h"
#include "Database.h"

class Insert : public Statement {
public:
    Insert(const string &input, Database *database)
            : Statement(tryToGetTableFromQuery(input, database), input), database(database) {}

    void execute() override {
        handleInput(inputQuery);
    }

private:
    Database *database;

    void handleInput(const string &input) const {
        smatch matches;
        if (regex_search(input, matches, getRegexPattern()))
            executingQuery(matches);
        else
            runtimeErrors(input);
    }


    regex getRegexPattern() const {
        return regex(
                "^\\s*insert\\s+into\\s+([a-zA-Z_]+)\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\")\\s*(?:\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\")\\s*)*)\\s*\\)\\s+values\\s*\\(\\s*((?:\\'\\w+\\'|\\\"\\w+\\\")\\s*(?:\\,\\s*(?:\\'\\w+\\'|\\\"\\w+\\\"))*)*\\s*\\)\\s*$",
                regex_constants::icase);
    }

    void executingQuery(const smatch &matches) const {
        string tableName = matches[1];
        vector<string> listOfColumns = StringManipulator::instance().splitString(matches[2], ',');
        vector<string> values = StringManipulator::instance().splitString(matches[3], ',');

        checkIfAllColumnsAreThere(listOfColumns, values.size());

        StringManipulator::instance().removeQuotesAndLeadingSpaces(values);

        Record record = Record();
        for (const string &val: values) {
            record.addData(val);
        }

        table->addRecord(record);

        cout << *table;
    }

    void checkIfAllColumnsAreThere(vector<string> listOfColumns, const size_t listOfValuesSize) const {
        if (listOfColumns.size() != table->getTableHeaders().size())
            throw EBadArgumentsException(
                    "[RUNTIME_ERROR] Every column from " + table->getName() + " should be selected while inserting");

        if (listOfValuesSize != listOfColumns.size())
            throw EBadArgumentsException(
                    "[RUNTIME_ERROR] Every column from " + table->getName() + " should be given value.");

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

    void runtimeErrors(const string &input) const {
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
        if (regex_match(query, regex(".*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*select.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*create.*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple INSERT keywords not allowed within the same statement.");
        }
    }

    string getTableNameForErrorMsg(const string &query) const {
        std::smatch matches;
        if (regex_search(query, matches, regex("into\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        return "";
    }

    Table *tryToGetTableFromQuery(const string &query, const Database *database) const {
        checkForSyntaxErrors(query);

        Table *table = nullptr;

        std::smatch matches;
        if (regex_search(query, matches, regex("into\\s+(\\w+)", regex_constants::icase))) {
            table = database->tryGettingTableByNameCaseI(matches[1]);
        }

        if (!table)
            throw EBadArgumentsException("[RUNTIME_ERROR] Table " + getTableNameForErrorMsg(query) + " not found.");
        return table;
    }
};


#endif //PRVIPROJEKAT_INSERT_H
