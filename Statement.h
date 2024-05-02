#ifndef PRVIPROJEKAT_STATEMENT_H
#define PRVIPROJEKAT_STATEMENT_H


#include "Table.h"

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


    virtual void executingQuery(const smatch &matches) const = 0;

    virtual const regex getRegexPattern() const = 0;

    virtual const regex getRegexForFindingTable() const = 0;

    virtual void runtimeErrors(const string &input) const = 0;

//    virtual void checkForSyntaxErrors(const string &query) const = 0;

};


#endif //PRVIPROJEKAT_STATEMENT_H
