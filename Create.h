#ifndef PRVIPROJEKAT_CREATE_H
#define PRVIPROJEKAT_CREATE_H


#include "Statement.h"
#include "StringManipulator.h"
#include "Database.h"

class Create : public Statement {
public:
    Create(const string &input, Database *database) : Statement(
            new Table(getTableNameFromCreateQuery(input)), input), database(database) {}

    void init() override {
        validate(inputQuery);
    }


private:
    Database *database;

    bool validate(const std::string &input) override {
        std::smatch matches;
        if (regex_search(input, matches,
                         regex("^\\s*create\\s+table\\s+(\\w+)\\s+\\(\\s*(\\w+\\s*(?:,\\s*\\w+)*)\\s*\\)",
                               regex_constants::icase))) {
            vector<string> tableColumns = StringManipulator::instance().splitString(matches[2], ',');
            for (const string &columnName: tableColumns) {
                table->addHeader(columnName);
            }
            if(this->database)
                addTableToDatabase();
            cout << *table;
            cout << "Table " << matches[1] << " successfully added to " << database->getName() << endl;
        } else if (regex_search(input, matches, regex("\\s*create\\s+table\\s+\\w+\\s*", regex_constants::icase))) {
            throw EBadArgumentsException("[ERROR] No table column names passed.");
        }
        return true;
    }

    void addTableToDatabase() const {

        this->database->addTable(*this->table);
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
