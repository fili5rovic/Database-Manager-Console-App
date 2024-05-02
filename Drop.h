#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement {
public:

    Drop(const string &input, Database *database)
            : Statement(tryToGetTableFromDropQuery(input, database), input), database(database) {}

    void execute() override {
        handleInput(inputQuery);
    }

private:
    Database *database;

    void handleInput(const string &input) const {
        std::smatch matches;
        if (regex_search(input, matches, getRegexPattern()))
            executingQuery(matches);
        else runtimeErrors(input);
    }

    regex getRegexPattern() const {
        return regex("^\\s*drop\\s+table\\s+([a-zA-Z_]+)\\s*",regex_constants::icase);
    }


    void executingQuery(const smatch) const {
        if (this->database)
            database->removeTable(table->getName());

        StringManipulator::instance().newMessageGreen("Table " + table->getName() + " successfully dropped.");
    }

    void runtimeErrors(const string &input) const {
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
