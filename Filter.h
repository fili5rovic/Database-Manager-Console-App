#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include <regex>
#include "Condition.h"
#include "Table.h"

class Filter {
public:

    Filter(const Table* t, const vector<string>& selectedColumns, const string &whereArgsString) { init(t,selectedColumns, whereArgsString); }



private:

    vector<vector<Condition*>> conditions; // outer vector stores conditions seperated by OR, inner by AND

    void applyFilter() {
        // run through all conditions and apply them to change the table records accordingly
    }

    // creates conditions
    void init(const Table* t,const vector<string>& selectedColumns, const string &whereArgs) {
        vector<string> orStrings = getOrStringsFromWhere(whereArgs);
        for(const string& conditionWithAnds : orStrings) {
            vector<string> conditionStrings = getAndStringsFromOrStrings(conditionWithAnds);
            for(const string& condStr : conditionStrings) {
                cout << condStr << endl;
            }
            cout << endl;
        }
    }

    // gets normal strings
    static vector<string> getAndStringsFromOrStrings(const string& whereStr) {
        std::regex regex_pattern(R"(\s+and\s+)", regex_constants::icase);

        std::sregex_token_iterator iter(whereStr.begin(), whereStr.end(), regex_pattern, -1);
        std::sregex_token_iterator end;

        vector<string> orStrings;

        while (iter != end) {
            orStrings.push_back(*iter);
            ++iter;
        }
        return orStrings;
    }
    // returns strings seperated by AND
    static vector<string> getOrStringsFromWhere(const string& whereStr) {
        std::regex regex_pattern(R"(\s+or\s+)", regex_constants::icase);

        std::sregex_token_iterator iter(whereStr.begin(), whereStr.end(), regex_pattern, -1);
        std::sregex_token_iterator end;

        vector<string> orStrings;

        while (iter != end) {
            orStrings.push_back(*iter);
            ++iter;
        }
        return orStrings;
    }


};


#endif //PRVIPROJEKAT_FILTER_H
