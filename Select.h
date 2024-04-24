#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include <regex>
#include "Statement.h"
#include "Filter.h"
#include "Errors.h"
#include "StringManipulator.h"


class Select : public Statement {
public:
    Select(const string& input) : inputQuery(input) {}


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
        return true;
    }

    void handleQuery(const string& inputQuery) {
        std::smatch matches;
        if(regex_match(inputQuery,Select::getRegexPattern())) {
            cout << "SELECT UPIT!" << endl;
        }
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
