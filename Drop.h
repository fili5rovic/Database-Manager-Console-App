#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement{
public:

    Drop(Table* table, const string& inputQuery, Database* database) : Statement(table,inputQuery), database(database) {}

    void execute() override {
        checkForSyntaxErrors(inputQuery);
        handleInput(inputQuery);
    }

private:
    Database *database;

    void handleInput(const string& input) {
        std::smatch matches;
        if (regex_search(input, matches,
                         regex("^\\s*drop\\s+table\\s+([a-zA-Z_]+)\\s*\\(\\s*(\\w+\\s*(?:,\\s*\\w+)*)\\s*\\)",
                               regex_constants::icase))) {
            if (this->database)
                database->removeTable(table->getName());

        }
        else runtimeErrors(input);
    }

    void runtimeErrors(const string &input) {
        if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
        }
        throw EBadArgumentsException("[SYNTAX_ERROR] Table drop error.");
    }

    void checkForSyntaxErrors(const string& query) const {
        if(!regex_match(query,regex(".*\\s+table.*",regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No TABLE keyword");
    }

};


#endif //PRVIPROJEKAT_DROP_H
