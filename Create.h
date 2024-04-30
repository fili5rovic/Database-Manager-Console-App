#ifndef PRVIPROJEKAT_CREATE_H
#define PRVIPROJEKAT_CREATE_H


#include "Statement.h"

class Create : public Statement{
public:
    Create(const string& input) : Statement(new Table(getTableNameFromCreateQuery(input)), input) {}

    void init() override {
        validate(inputQuery);
    }

private:

    bool validate(const std::string &str) override {
        cout << "HERE: " << str << endl;
        std::smatch matches;
        if(regex_search(str,matches,regex("^\\s*create\\s+table\\s+\\w+\\s+\\(\\s*((\\w+)\\s*(?:,\\s*\\w+)*)\\s*\\)",regex_constants::icase))) {
            for (int i = 0; i < matches.length(); ++i) {
                cout << "MATCH " << i << " " << matches[i] << endl;
            }
        }
        return true;
    }



    string getTableNameFromCreateQuery(const string& query) {
        std::smatch matches;
        vector<string> names;
        if (regex_search(query, matches, regex("^\\s*create\\s+table\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        throw EBadArgumentsException("[SYNTAX_ERROR] Table name " + matches[1].str() + " not allowed.");
    }

};


#endif //PRVIPROJEKAT_CREATE_H
