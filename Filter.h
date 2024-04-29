#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include <regex>
#include "Condition.h"
#include "Table.h"
#include "Equal.h"
#include "NotEqual.h"

class Filter {
public:

    Filter(const Table *t, const vector<string> &selectedColumns, const string &whereArgsString) {
        init(t, selectedColumns, whereArgsString);
    }


private:

    vector<vector<shared_ptr<Condition>>> conditions; // outer vector stores conditions seperated by OR, inner by AND

    shared_ptr<Table> applyFilter() {
        shared_ptr<Table> finalTable;
        shared_ptr<Table> tempTable;
        for(const auto& vec : conditions) {
            for(const auto& cond : vec) {
                tempTable = cond->getConditionedTable();
            }
            for(const auto& rec : tempTable->getTableRecords()) {
                finalTable->addRecord(rec);
            }
        }
        return finalTable;
    }

    const string getCorrectMatch(const std::smatch& matches) const {
        string value = "-1"; // if it stays -1 something's wrong
        if (matches[2].length() != 0)
            value = matches[2];
        else if (matches[3].length() > 0)
            value = matches[3];
        else if (matches[4].length() > 0)
            value = matches[4];
        else cerr << "PARSING ERROR!" << endl;
        return value;
    }

    // creates conditions, only called during object creation
    void init(const Table *t, const vector<string> &selectedColumns, const string &whereArgs) {
        vector<string> orStrings = getOrStringsFromWhere(whereArgs);
        for (const string &conditionWithAnds: orStrings) {
            shared_ptr<Table> tempTable = std::make_shared<Table>(*t);
            vector<string> conditionStrings = getAndStringsFromOrStrings(conditionWithAnds);
            vector<shared_ptr<Condition>> innerVector;
            for (string &condStr: conditionStrings) {
                cout << condStr << " ";
                string valueStr;
                smatch matches;
                shared_ptr<Condition> e;
                if (regex_search(condStr, matches,regex("^(\\w+)\\s+\\=\\s+(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                    valueStr = getCorrectMatch(matches);
                    e = std::make_shared<Equal>(tempTable, matches[1], valueStr);
                } else if(regex_search(condStr, matches,regex("^(\\w+)\\s+(?:\\!\\=|\\<\\>)\\s+(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                    valueStr = getCorrectMatch(matches);
                    e = std::make_shared<NotEqual>(t, matches[1], valueStr);
                }
                tempTable = e->getConditionedTable();
                innerVector.push_back(e);
            }
            this->conditions.push_back(innerVector);
            cout << endl;
        }
    }

    // gets normal strings
    static vector<string> getAndStringsFromOrStrings(const string &whereStr) {
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
    static vector<string> getOrStringsFromWhere(const string &whereStr) {
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
