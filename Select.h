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
    Select(const string& input) : inputQuery(input){}

    // TODO: think how to connect database with Select without passing it.
    // FIND TABLE BEFORE INIT AND PASS IT AS AN ARGUMENT
    void init() override{
        if(!validate(inputQuery)) {
            cout << "Select query failed." << endl;
        }
    }

    static const regex& getRegexPattern() {
        return regexPattern;
    }


private:
    static const regex regexPattern;
    const string inputQuery;
    Filter filter;


    bool validate(const std::string &str) override {
        // multiple keywords
        if(regex_match(str,regex(".*insert.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected INSERT with SELECT");
        } else if(regex_match(str,regex(".*create.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected CREATE with SELECT");
        } else if(regex_match(str,regex(".*insert.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected INSERT with SELECT");
        } else if(regex_match(str,regex(".*into.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected INTO with SELECT");
        } else if(regex_match(str,regex(".*delete.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected DELETE with SELECT");
        } else if(regex_match(str,regex(".*table.*",regex_constants::icase))) {
            throw EMultipleKeywordsException("[ERROR] Detected TABLE with SELECT");
        }
        // select has no arguments
        if(!regex_match(str,regex(".*select\\s+(?:\\w+|\\*)",regex_constants::icase))) {
            throw EMissingArgumentsException("[ERROR] SELECT has no arguments.");
        }

        // from not detected
        if(!regex_match(str,regex(".*from.*",regex_constants::icase))) {
            throw EMissingKeywordsException("[ERROR] No FROM keyword specified.");
        } // table name not detected
        else if(!regex_match(str,regex(".*from\\s+\\w+$",regex_constants::icase))) {
            throw EMissingArgumentsException("[ERROR] FROM has no arguments.");
        }

        // set table
        std::smatch matches;
        if(regex_search(inputQuery,matches,regex("\\s+from\\s+(\\w+)+"))) {
            cout << "TABLE NAME:" << matches[1] << endl;
        }

        return true;
    }

    void handleQuery(const string& inputQuery) {
        std::smatch matches;
        if(regex_search(inputQuery,matches,Select::getRegexPattern())) {
            cout << "SELECT UPIT yes" << endl;
            cout << "Number of groups: " << matches.size() - 1 << endl; // Exclude the whole match
            for (size_t i = 1; i < matches.size(); ++i) {
                cout << "Group " << i << ": " << matches[i] << endl;
            }


            string argumentsStr = matches[1];
            StringManipulator::removeSpaces(argumentsStr);
            string fromTable = matches[3];

            vector<string> arguments = StringManipulator::splitString(argumentsStr, ',');
            for(const auto& str:arguments) {
                cout << str << endl;
            }

            cout << "Table: " << fromTable << endl;
        }
    }
};


#endif //PRVIPROJEKAT_SELECT_H
