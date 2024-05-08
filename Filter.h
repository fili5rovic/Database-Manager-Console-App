#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include <regex>
#include "Condition.h"
#include "Table.h"
#include "Equal.h"
#include "NotEqual.h"

class Filter {
public:

    Filter(const shared_ptr<Table> t, const string &whereArgsString)  {
        init(t, whereArgsString);
    }

    shared_ptr<Table> getFilteredTable() {
        return this->table;
    }


private:
    // outer vector stores conditions seperated by OR, inner by AND
    vector<vector<shared_ptr<Condition>>> conditions;
    shared_ptr<Table> table;


    // creates conditions, only called during object creation
    void init(const shared_ptr<Table> t, const string &whereArgs) {
        cout << "Filtering table with WHERE: " << whereArgs << endl;

        if(whereArgs.empty()) {
            table = make_shared<Table>(*t);
            return;
        }
        setHeaderAndNameFromParameterTable(t);
        vector<string> orStrings = getOrStringsFromWhere(whereArgs);
        for (const string &conditionWithAnds: orStrings) {
            Table* tempTable = new Table(*t);
            vector<string> conditionStrings = getAndStringsFromOrStrings(conditionWithAnds);
            vector<shared_ptr<Condition>> innerVector;
            for (string &condStr: conditionStrings) {
                shared_ptr<Condition> condition;
                string valueStr;

                smatch matches;
                if (regex_search(condStr, matches,regex("^((?:\\w+\\.)?\\w+)\\s*\\=\\s*(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                    valueStr = getCorrectMatch(matches);
                    condition = std::make_shared<Equal>(tempTable, matches[1], valueStr);
                } else if(regex_search(condStr, matches,regex("^((?:\\w+\\.)?\\w+)\\s*(?:\\!\\=|\\<\\>)\\s*(?:(\\w+)|'(\\w+)'|\"(\\w+)\")$", regex_constants::icase))) {
                    valueStr = getCorrectMatch(matches);
                    condition = std::make_shared<NotEqual>(tempTable, matches[1], valueStr);
                } else
                    throw EBadArgumentsException("[SYNTAX_ERROR] Invalid WHERE syntax.");

                tempTable = condition->getConditionedTable();
                innerVector.push_back(condition);
            }
            this->conditions.push_back(innerVector);
        }

        updateTableRows();
    }

    void updateTableRows() const {
        Table* tempTable;
        for(const auto& vec : conditions) {
            // last vector has the result of all ANDs between an OR
            tempTable = vec.at(vec.size()-1)->getConditionedTable();
            for(Record& rec : tempTable->getTableRecordsByReference()) {
                table->addRecord(rec);
            }
        }
    }


    const string getCorrectMatch(const std::smatch& matches) const {
        for(int i = 2; i < matches.length();i++) {
            if(matches[i].length() > 0)
                return matches[i];
        }
        return "-1";
    }


    void setHeaderAndNameFromParameterTable(const shared_ptr<Table> t) {
        table = make_shared<Table>("RESULT");
        for(const string& header : t->getTableHeaders()) {
            table->addHeader(header);
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
