#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include <regex>
#include "Statement.h"
#include "Filter.h"
#include "Errors.h"
#include "StringManipulator.h"
#include "Database.h"


class Select : public Statement {
public:
    Select(const string &input,Database* database) : Statement(input, database){}

private:
    void executingQuery(const smatch &matches) const override {
        string argumentsStr = matches[1];
        StringManipulator::removeSpaces(argumentsStr);
        string fromTableStr = matches[2];
        string whereStr = matches[3];

        vector<string> arguments = StringManipulator::splitString(argumentsStr, ',');
        try {
            Filter f(this->table, arguments, whereStr);
            cout << *f.getTableWithAppliedFilter();
        } catch(EInvalidColumnNameException& e) {
            StringManipulator::instance().newMessageRed(e.what());
        }
    }


    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override {
        StringManipulator::instance().newMessageRed("[RUNTIME_ERROR] Runtime error.");
    }

    void checkForSyntaxErrors(const string &query) const {
        // from not detected
        if (!regex_match(query, regex(".*\\s+from\\s*.*", regex_constants::icase))) {
            throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
        } // table name not detected
        else if (!regex_search(query, regex(".*\\s+from\\s+\\w+\\s*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
        }
        // multiple keywords
        if (regex_match(query, regex(".*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*insert.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with SELECT not allowed.");
        } else if (regex_match(query, regex(".*into.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with SELECT not allowed.");
        } else if (regex_match(query, regex(".*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*table.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*select.*select.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple SELECT keywords not allowed.");
        }
        // select has no arguments
        if (regex_match(query, regex("^\\s*select\\s+(?:\\,\\s*|\\s*)*\\s*from.*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] SELECT has no arguments.");
        }
    }
    //</editor-fold>


    //<editor-fold desc="Getters">
    regex getRegexPattern() const {
        return regex(R"(^\s*select\s+((?:\w+|\*)(?:\s*,\s*(?:\w+|\*))*)\s+from\s+(\w+)+\s*(?:where\s+((\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+)(?:\s+(and|or)\s*(\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+))*))?)", regex_constants::icase);;
    }

    regex getRegexForFindingTable() const override {
        return regex("from\\s+(\\w+)", regex_constants::icase);
    }
    //</editor-fold>


};


#endif //PRVIPROJEKAT_SELECT_H
