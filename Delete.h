#ifndef PRVIPROJEKAT_DELETE_H
#define PRVIPROJEKAT_DELETE_H


#include "Statement.h"
#include "Filter.h"

class Delete : public Statement {
public:
    Delete(const string &input, shared_ptr<Database> database);
private:
    virtual void executingQuery(const smatch &matches) const;



    void runtimeErrors(const string &input) const override  {
        if(regex_search(input,regex("^\\s*delete\\s+from\\s*(\\w+)\\s*",regex_constants::icase)))
            throw EBadArgumentsException("[RUNTIME_ERROR] Table names can't contain numbers."); // may never enter idk
        throw EBadArgumentsException("[RUNTIME_ERROR] Not yet implemented.");
    }

    void checkForSyntaxErrors(const string &query) const override {
        // from not detected
        if (!regex_match(query, regex(".*\\s+from\\s*.*", regex_constants::icase))) {
            throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
        } // table name not detected
        else if (!regex_search(query, regex(".*\\s+from\\s+(\\w+)\\s*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
        }
        // multiple keywords
        if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with DELETE not allowed.");
        } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with DELETE not allowed.");
        } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with DELETE not allowed.");
        } else if (regex_match(query, regex(".*\\bselect\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] select with DELETE not allowed.");
        } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with DELETE not allowed.");
        } else if (regex_match(query, regex(".*\\bdelete\\b.*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple DELETE keywords not allowed.");
        } else if (regex_match(query, regex(".*\\bfrom\\b.*\\bfrom\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple FROM keywords not allowed.");
        }
    }


    regex getRegexPattern() const override {
        return regex("^\\s*delete\\s+from\\s*([a-zA-Z_]+)\\s*(?:where\\s+((\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*(\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?",regex_constants::icase);
    }

    regex getRegexForFindingTable() const override {
        return regex("^\\s*delete\\s+from\\s*(\\w+)\\s*",regex_constants::icase);
    }
};


#endif //PRVIPROJEKAT_DELETE_H
