#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"
#include "Database.h"

class Statement {
public:

    virtual void execute(){
        table = tryToGetTableFromQuery(inputQuery, database, getRegexForFindingTable());
        handleQuery(inputQuery);
    }

protected:
    Table *table;
    const string inputQuery;
    Database *database;

    Statement(const string &inputQuery, Database *database) : database(database), inputQuery(inputQuery), table(nullptr) {}

    void handleQuery(const string &input) const {
        smatch matches;
        if (regex_search(input, matches, getRegexPattern()))
            executingQuery(matches);
        else
            runtimeErrors(input);
    }

    virtual void executingQuery(const smatch &matches) const = 0;

    virtual regex getRegexPattern() const = 0;

    virtual regex getRegexForFindingTable() const = 0;

    virtual void runtimeErrors(const string &input) const = 0;

    virtual void checkForSyntaxErrors(const string &query) const {};


    Table *tryToGetTableFromQuery(const string &query, const Database *database, const regex &regexPattern) const {
        checkForSyntaxErrors(query);

        Table *currTable = nullptr;

        std::smatch matches;
        if (regex_search(query, matches, regexPattern)) { // here
            if (matches.size() > 1) {
                currTable = database->tryGettingTableByNameCaseI(matches[1]);
            }
        }

        if (!currTable) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Table not found.");
        }
        return currTable;
    }

};


#endif //PRVIPROJEKAT_STATEMENT_H
