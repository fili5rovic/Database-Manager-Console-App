#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"
#include "Database.h"

class Statement {
public:

    void execute() {
        handleQuery(inputQuery);
    };

protected:
    Table* table;
    const string inputQuery;

    Statement(Table* table, const string& inputQuery) : table(table), inputQuery(inputQuery) {}

    void handleQuery(const string &input) const {
        smatch matches;
        if (regex_search(input, matches, getRegexPattern()))
            executingQuery(matches);
        else
            runtimeErrors(input);
    }

    Table* tryToGetTableFromQuery(const string &query, const Database *database, const regex& regexPattern) const {
        if (!database) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Database is null.");
        }
        checkForSyntaxErrors(query);

        Table *currTable = nullptr;

        std::smatch matches;
        if (regex_search(query, matches, regexPattern)) { // here
            if (matches.size() > 1) {
                currTable = database->tryGettingTableByNameCaseI(matches[1]);
            }
        }

        if (!currTable) {
            string tableName = matches.size() > 1 ? matches[1].str() : "Unknown";
            throw EBadArgumentsException("[RUNTIME_ERROR] Table " + tableName + " not found.");
        }
        return currTable;
    }


    virtual void executingQuery(const smatch &matches) const = 0;

    virtual regex getRegexPattern() const = 0;

    virtual regex getRegexForFindingTable() const = 0;

    virtual void runtimeErrors(const string &input) const = 0;

    virtual void checkForSyntaxErrors(const string &query) const {};

};


#endif //PRVIPROJEKAT_STATEMENT_H
