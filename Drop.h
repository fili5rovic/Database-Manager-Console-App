#ifndef PRVIPROJEKAT_DROP_H
#define PRVIPROJEKAT_DROP_H


#include "Statement.h"
#include "Database.h"

class Drop : public Statement {
public:
    Drop(const string &input, Database *database) : Statement(input, database) {}

private:
    void executingQuery(const smatch& matches) const override {
        if (this->database)
            database->removeTable(table->getName());

        StringManipulator::instance().newMessageGreen("Table " + table->getName() + " successfully dropped.");
    }

    //<editor-fold desc="Error Handling">
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
        if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bselect\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] SELECT with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with DROP not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with INSERT not allowed within the same statement.");
        } else if (regex_match(query, regex(".*\\bdrop\\b.*\\bdrop\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException(
                    "[SYNTAX_ERROR] Multiple DROP keywords not allowed within the same statement.");
        }

    }
    //</editor-fold>

    //<editor-fold desc="Getters">
    regex getRegexPattern() const override {
        return regex("^\\s*drop\\s+table\\s+([a-zA-Z_]+)\\s*",regex_constants::icase);
    }

    regex getRegexForFindingTable() const override {
        return regex("table\\s+(\\w+)", regex_constants::icase);
    }
    //</editor-fold>

};


#endif //PRVIPROJEKAT_DROP_H
