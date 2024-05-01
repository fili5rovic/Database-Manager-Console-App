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
    Select(const string &input,const Database* database) : Statement(tryToGetTableFromSelectQuery(input, database), input){}


    void execute() override {
        checkForSyntaxErrors(inputQuery);
        handleQuery(inputQuery);
    }


    static const regex &getRegexPattern() {
        return regexPattern;
    }


private:
    static const regex regexPattern;

    static void checkForSyntaxErrors(const string &query) {
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


    void handleQuery(const string &inputQuery) {
        std::smatch matches;
        if (regex_search(inputQuery, matches, Select::getRegexPattern())) {
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

        } else {
            StringManipulator::instance().newMessageRed("[ERROR] Syntax error");
        }
    }

    string getTableNameForErrorMsg(const string &query) const{
        std::smatch matches;
        if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        return "";
    }

    Table *tryToGetTableFromSelectQuery(const string &query, const Database* database) const {
        checkForSyntaxErrors(query);

        Table *table = nullptr;
        vector<string> names;

        std::smatch matches;
        // without join
        if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            table = database->tryGettingTableByNameCaseI(matches[1]);
        }
            // with join on
        else if (regex_search(query, matches,
                              regex("from\\s+(\\w+)\\s+(\\w+)\\s+(?:(?:inner\\s+)?join\\s+(\\w+)\\s+(\\w+)\\s+(?:on\\s+(?:\\w+\\.\\w+\\s*\\=\\s*\\w+\\.\\w+\\s*)))+",
                                    regex_constants::icase))) {
            // make a new table that's joined
        }
            // with join using
        else if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            // make a new table that's joined
        }

        if (!table)
            throw EBadArgumentsException("[RUNTIME_ERROR] Table " + getTableNameForErrorMsg(query) + " not found.");
        return table;
    }


};


#endif //PRVIPROJEKAT_SELECT_H
