#ifndef PRVIPROJEKAT_CREATE_H
#define PRVIPROJEKAT_CREATE_H


#include "Statement.h"
#include "StringManipulator.h"
#include "Database.h"

class Create : public Statement {
public:
    Create(const string &input, Database *database)
            : Statement(new Table(getTableNameFromCreateQuery(input)), input), database(database) {}


    void execute() override {
        handleQuery(inputQuery);
    }


private:
    Database *database;

    void handleQuery(const string &input) {
        smatch matches;
        if (regex_search(input, matches, getRegexPattern()))
            executingQuery(matches);
        else
            runtimeErrors(input);
    }

    void executingQuery(const smatch &matches) const {
        vector<string> tableColumns = StringManipulator::instance().splitString(matches[2], ',');
        for (string &columnName: tableColumns) {
            StringManipulator::removeSpaces(columnName);
            table->addHeader(columnName);
        }
        if (this->database)
            addTableToDatabaseIfUnique();
        cout << *table;
    }

    const regex getRegexPattern() const {
        return regex("^\\s*create\\s+table\\s+([a-zA-Z_]+)\\s*\\(\\s*(\\w+\\s*(?:,\\s*\\w+)*)\\s*\\)",
                     regex_constants::icase);
    }


    void addTableToDatabaseIfUnique() const {
        if (this->database->tryGettingTableByNameCaseI(table->getName()))
            throw EBadArgumentsException("[RUNTIME_ERROR] Table with identical name exists inside a database!");
        this->database->addTable(*this->table);
    }


    void runtimeErrors(const string &input) const {
        std::smatch matches;
        if (regex_match(input, regex("\\s*create\\s+table\\s+\\w+\\s*", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] No table column names passed.");
        }
        if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
        }

        throw EBadArgumentsException("[SYNTAX_ERROR] Table creation error.");
    }


    void checkForSyntaxErrors(const string &query) const {
        if (!regex_match(query, regex(".*\\s+table.*", regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No TABLE keyword");

        // multiple keywords
        if (regex_match(query, regex(".*insert.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with CREATE not allowed within the same statement.");
        } else if (regex_match(query, regex(".*select.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with CREATE not allowed within the same statement.");
        } else if (regex_match(query, regex(".*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with CREATE not allowed within the same statement.");
        } else if (regex_match(query, regex(".*create.*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple CREATE keywords not allowed within the same statement.");
        }

    }


    string getTableNameFromCreateQuery(const string &query) {
        checkForSyntaxErrors(query);

        std::smatch matches;
        vector<string> names;                           // it was s+ in the end instead of s*
        if (regex_search(query, matches, regex("^\\s*create\\s+table\\s+(\\w+)(?:s*|\\()", regex_constants::icase))) {
            if (!regex_match(matches[1].str(), regex("[a-zA-Z_]+", regex_constants::icase)))
                throw EBadArgumentsException("[RUNTIME_ERROR] Table name " + matches[1].str() + " not allowed.");
            return matches[1];
        }
        throw EBadArgumentsException("[RUNTIME_ERROR] Table name not allowed.");
    }

};


#endif //PRVIPROJEKAT_CREATE_H
