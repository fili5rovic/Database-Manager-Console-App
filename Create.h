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

    bool validate(const std::string &input) override {
        cout << "HERE: " << input << endl;
        std::smatch matches;
        if (regex_search(input, matches,regex("^\\s*create\\s+table\\s+(\\w+)\\s+\\(\\s*(\\w+\\s*(?:,\\s*\\w+)*)\\s*\\)", regex_constants::icase))) {
            for (int i = 0; i < matches.length(); ++i) {
                cout << "MATCH " << i << " " << matches[i] << endl;
            }
        } else if(regex_search(input, matches, regex("\\s*create\\s+table\\s+\\w+\\s*", regex_constants::icase))) {
            throw EBadArgumentsException("[ERROR] No table column names passed.");
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
