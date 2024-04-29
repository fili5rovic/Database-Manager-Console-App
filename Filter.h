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

    Table* getTableWithAppliedFilter() {
        Table* finalTable = new Table("");
        Table* tempTable = new Table("");
        for(const auto& vec : conditions) {
            tempTable = vec.at(vec.size()-1)->getConditionedTable();
//            for(const auto& cond : vec) {
//                tempTable = cond->getConditionedTable();
//            }
            for(const auto& rec : tempTable->getTableRecords()) {
                finalTable->addRecord(rec);
            }
        }
        return finalTable;
    }
private:
    vector<vector<shared_ptr<Condition>>> conditions; // outer vector stores conditions seperated by OR, inner by AND


    const string getCorrectMatch(const std::smatch& matches) const {
        for(int i = 2; i < matches.length();i++) {
            if(matches[i].length() > 0)
                return matches[i];
        }
        return "-1";
    }

    // creates conditions, only called during object creation
    void init(const Table *t, const vector<string> &selectedColumns, const string &whereArgs) {
        vector<string> orStrings = getOrStringsFromWhere(whereArgs);
        for (const string &conditionWithAnds: orStrings) {
            Table* tempTable = new Table(*t);
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
                    e = std::make_shared<NotEqual>(tempTable, matches[1], valueStr);
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
