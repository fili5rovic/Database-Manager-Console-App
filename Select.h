#ifndef PRVIPROJEKAT_SELECT_H
#define PRVIPROJEKAT_SELECT_H

#include <regex>
#include "Statement.h"
#include "Filter.h"

class Select : public Statement {
public:
    Select(const string& input) : inputQuery(input) {}

    static const regex& getRegexPattern() {
        return regexPattern;
    }


private:
    static const regex regexPattern;
    const string inputQuery;
    Filter filter;

    bool validate(const std::string &str) override {
        return true;
    }
};


#endif //PRVIPROJEKAT_SELECT_H
