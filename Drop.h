#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement {
public:

    Drop(const string &inputQuery, Database *database)
            : Statement(tryToGetTableFromDropQuery(inputQuery, database), inputQuery), database(database) {}

    void execute() override {
        handleInput(inputQuery);
    }

private:
    Database *database;

    void handleInput(const string &input) {
        std::smatch matches;
        if (regex_search(input, matches,
                         regex("^\\s*drop\\s+table\\s+([a-zA-Z_]+)\\s*",
                               regex_constants::icase))) {
            if (this->database)
                database->removeTable(table->getName());
            StringManipulator::instance().newMessageGreen("Table " + table->getName() + " successfully dropped.");

        } else runtimeErrors(input);
    }

    void runtimeErrors(const string &input) {
        if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
        }
        throw EBadArgumentsException("[SYNTAX_ERROR] Table drop error.");
    }

    void checkForSyntaxErrors(const string &query) const {
        if (!regex_match(query, regex(".*\\s+table.*", regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No TABLE keyword");
        if (regex_match(query,regex(".*\\s+table\\s*$", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] DROP TABLE has no argument.");
        }
    }

    string getTableNameForErrorMsg(const string &query) const {
        std::smatch matches;
        if (regex_search(query, matches, regex("table\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        return "";
    }

    Table *tryToGetTableFromDropQuery(const string &query, const Database *database) const {
        checkForSyntaxErrors(query);

        Table *table = nullptr;
        vector<string> names;

        std::smatch matches;
        if (regex_search(query, matches, regex("table\\s+(\\w+)", regex_constants::icase))) {
            table = database->tryGettingTableByNameCaseI(matches[1]);
        }

        if (!table)
            throw EBadArgumentsException("[RUNTIME_ERROR] Table " + getTableNameForErrorMsg(query) + " not found.");
        return table;
    }

};


#endif //PRVIPROJEKAT_DROP_H
