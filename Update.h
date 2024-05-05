#ifndef PRVIPROJEKAT_UPDATE_H
#define PRVIPROJEKAT_UPDATE_H


#include "Statement.h"
#include "Filter.h"

class Update : public Statement {
public:
    Update(const string &input, Database *database) : Statement(input, database) {}

private:

    void executingQuery(const std::smatch &matches) const override {
        string tableName = matches[1];
        string setArgStr = matches[2];
        string whereStr = matches[3];

        vector<string> setArgs = StringManipulator::instance().splitString(setArgStr, ',');
        // trim them
        for (string &str: setArgs) {
            str = StringManipulator::instance().trim(str);
        }

        Table *filterTable;

        try {
            Filter f(this->table, whereStr);
            filterTable = f.getFilteredTable();
        } catch (EInvalidColumnNameException &e) {
            cout << e.what();
        }

        vector<int> indices;
        indices.reserve(filterTable->getTableRecords().size());
        for (const Record &rec: filterTable->getTableRecords()) {
            indices.push_back(table->getRecordIndex(rec.getData()));
        }

        for (int index: indices) {
            for (const string &setArg: setArgs) {
                std::smatch matches1;
                if (regex_search(setArg, matches1,
                                 regex("^\\s*(\\w+)\\s*\\=\\s*[\'\"](\\w*|\\s)[\'\"]", regex_constants::icase))) {
                    string columnName = matches1[1];
                    string columnValue = matches1[2];

                    StringManipulator::instance().trim(columnName);
                    StringManipulator::instance().trim(columnValue);




                    int columnIndex = -1;
                    for (int i = 0; i < filterTable->getTableHeaders().size(); i++) {
                        if (regex_match(columnName,
                                        regex(filterTable->getTableHeaders().at(i), regex_constants::icase))) {
                            columnIndex = i;
                            break;
                        }
                    }
                    table->getRecordByIndex(index).getDataReference()[columnIndex] = columnValue;
                }
            }
        }
        cout << *table;

        printAffectedRowsMessage(indices.size());
    }

    void printAffectedRowsMessage(int num) const {
        string msg;
        if (num == 0) {
            msg = "No rows affected.";
        } else if (num == 1) {
            msg = "1 row was affected through this change.";
        } else {
            msg = to_string(num) + " rows were affected through this update.";
        }
        StringManipulator::instance().newMessage(msg);
    }

    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override {
        throw EBadArgumentsException("[RUNTIME_ERROR] Bad table probably lol");
    }

    void checkForSyntaxErrors(const std::string &query) const override {
        // set not detected
        if (!regex_match(query, regex(".*\\s+set\\s*.*", regex_constants::icase))) {
            throw EMissingKeywordsException("[SYNTAX_ERROR] No SET keyword specified.");
        } // table name not detected
        else if (!regex_search(query, regex(".*\\s+set\\s+\\w+\\s*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] SET has no arguments.");
        }
        // multiple keywords
        if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with UPDATE not allowed.");
        } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with UPDATE not allowed.");
        } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with UPDATE not allowed.");
        } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with UPDATE not allowed.");
        } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with UPDATE not allowed.");
        } else if (regex_match(query, regex(".*\\bupdate\\b.*\\bupdate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple UPDATE keywords not allowed.");
        }
        // update has no arguments
        if (!regex_search(query, regex("^\\s*update\\s+(\\w+)", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] UPDATE has no arguments.");
        }
    }
    //</editor-fold>

    //<editor-fold desc="Getters">
    regex getRegexForFindingTable() const override {
        return regex("^\\s*update\\s+(\\w+)", regex_constants::icase);
    }


    regex getRegexPattern() const override {
        return regex(
                "^\\s*update\\s+([a-zA-Z_]*)\\s+set\\s+(\\w+\\s*\\=\\s*(?:\\\"(?:\\w*|\\s)\\\"|\\'(?:\\w*|\\s)\\')\\s*(?:\\,\\s*\\w+\\s*\\=\\s*(?:\\\"(?:\\w*|\\s)\\\"|\\'(?:\\w*|\\s)\\')\\s*)*)(?:where\\s+((\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*(\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?",
                regex_constants::icase);
    }
    //</editor-fold>
};


#endif //PRVIPROJEKAT_UPDATE_H
