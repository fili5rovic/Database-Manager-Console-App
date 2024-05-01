#ifndef PRVIPROJEKAT_CREATE_H
#define PRVIPROJEKAT_CREATE_H


#include "Statement.h"
#include "StringManipulator.h"
#include "Database.h"

class Create : public Statement {
public:
    Create(const string &input, Database *database) : Statement(
            new Table(getTableNameFromCreateQuery(input)), input), database(database) {}

    void execute() override {
        handleQuery(inputQuery);
    }


private:
    Database *database;

    void handleQuery(const string &input) {
        std::smatch matches;
        if (regex_search(input, matches,
                         regex("^\\s*create\\s+table\\s+(\\w+)\\s+\\(\\s*(\\w+\\s*(?:,\\s*\\w+)*)\\s*\\)",
                               regex_constants::icase))) {
            vector<string> tableColumns = StringManipulator::instance().splitString(matches[2], ',');
            for (string &columnName: tableColumns) {
                StringManipulator::removeSpaces(columnName);
                table->addHeader(columnName);
            }
            if(this->database)
                this->database->addTable(*this->table);
            cout << *table;
        } else {
            runtimeErrors(input);
        }
    }

    void runtimeErrors(const string& input) {
        std::smatch matches;
        if (regex_search(input, matches, regex("\\s*create\\s+table\\s+\\w+\\s*", regex_constants::icase))) {
            throw EBadArgumentsException("[ERROR] No table column names passed.");
        }
    }

    string getTableNameFromCreateQuery(const string &query) {
        std::smatch matches;
        vector<string> names;
        if (regex_search(query, matches, regex("^\\s*create\\s+table\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        throw EBadArgumentsException("[SYNTAX_ERROR] Table name " + matches[1].str() + " not allowed.");
    }

};


#endif //PRVIPROJEKAT_CREATE_H
