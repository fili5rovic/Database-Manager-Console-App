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
    Select(const string &input, Table *t) : Statement(t), inputQuery(input) {}


    ~Select() { delete filter;  }

    // FIND TABLE BEFORE INIT AND PASS IT AS AN ARGUMENT
    void init() override {
        validate(inputQuery);

    }

    static const regex &getRegexPattern() {
        return regexPattern;
    }


private:
    static const regex regexPattern;
    const string inputQuery;
    Filter* filter;


    static void checkForSyntaxErrors(const string &str) {
        // multiple keywords
        if (regex_match(str, regex(".*create.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with SELECT not allowed.");
        } else if (regex_match(str, regex(".*insert.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with SELECT not allowed.");
        } else if (regex_match(str, regex(".*into.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with SELECT not allowed.");
        } else if (regex_match(str, regex(".*delete.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with SELECT not allowed.");
        } else if (regex_match(str, regex(".*table.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with SELECT not allowed.");
        } else if (regex_match(str, regex(".*select.*select.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple SELECT keywords not allowed.");
        }
        // select has no arguments
        if (regex_match(str, regex("^\\s*select\\s+(?:\\,\\s*|\\s*)*\\s*from.*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] SELECT has no arguments.");
        }
    }


    bool validate(const string &str) override {
        checkForSyntaxErrors(str);
        handleQuery(str);
        return true;
    }




    void handleQuery(const string &inputQuery) {
        std::smatch matches;
        if (regex_search(inputQuery, matches, Select::getRegexPattern())) {
            cout << "Number of groups: " << matches.size() - 1 << endl;
            for (size_t i = 1; i < matches.size(); ++i) {
                cout << "Group " << i << ": " << matches[i] << endl;
            }

            string argumentsStr = matches[1];
            StringManipulator::removeSpaces(argumentsStr);
            string fromTableStr = matches[2];

            string whereStr = matches[3];

            vector<string> arguments = StringManipulator::splitString(argumentsStr, ',');

            //todo create filter with whereStr
            Filter f(this->table, arguments, whereStr);

            cout << *f.getTableWithAppliedFilter();

        } else {
            cout << StringManipulator::instance().REDCOLOR() << "ERROR: Syntax error"
                 << StringManipulator::instance().RESETCOLOR() << endl;
        }
    }
};


#endif //PRVIPROJEKAT_SELECT_H
