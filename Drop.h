#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement {
public:

    Drop(const string &input, Database *database)
            : Statement(tryToGetTableFromQuery(input, database, getRegexForFindingTable()), input), database(database) {}

private:
    Database *database;


    regex getRegexPattern() const override {
        return regex("^\\s*drop\\s+table\\s+([a-zA-Z_]+)\\s*",regex_constants::icase);
    }

    regex getRegexForFindingTable() const override {
        return regex("table\\s+(\\w+)", regex_constants::icase);
    }


    void executingQuery(const smatch& matches) const override {
        if (this->database)
            database->removeTable(table->getName());

        StringManipulator::instance().newMessageGreen("Table " + table->getName() + " successfully dropped.");
    }

    void runtimeErrors(const string &input) const override {
        if (!regex_match(this->table->getName(), regex("[a-zA-Z_]+", regex_constants::icase))) {
            throw EBadArgumentsException("[RUNTIME_ERROR] Invalid table name.");
        }
        throw EBadArgumentsException("[RUNTIME_ERROR] Table drop error.");
    }

    void checkForSyntaxErrors(const string &query) const {
        if (!regex_match(query, regex(".*\\s+table.*", regex_constants::icase)))
            throw EMissingKeywordsException("[SYNTAX_ERROR] No TABLE keyword");
        if (regex_match(query, regex(".*\\s+table\\s*$", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] DROP TABLE has no argument.");
        }
        // multiple keywords
        if (regex_match(query, regex(".*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*select.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*insert.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*drop.*drop.*", regex_constants::icase))) {
            throw EMultipleKeywordsException(
                    "[SYNTAX_ERROR] Multiple DROP keywords not allowed within the same statement.");
        }

    }

};


#endif //PRVIPROJEKAT_DROP_H
