#ifndef PRVIPROJEKAT_FILTER_H
#define PRVIPROJEKAT_FILTER_H

#include <regex>
#include "Condition.h"
#include "Table.h"
#include "Equal.h"
#include "NotEqual.h"

class Filter {
public:

    Filter(const Table *t, const vector<string> &selectedColumns, const string &whereArgsString) : selectedColumns(selectedColumns){
        init(t, whereArgsString);
    }

    Table* getTableWithAppliedFilter() {
        Table* finalTable = new Table(table->getName());
        this->selectedColumns = getProcessedSelectedColumns();

        for(int i = 0; i < table->getTableRecords().size(); i++)
            finalTable->addRecord(Record());


        for(const auto& selectedColumn : selectedColumns) {
            Table* tempTable = table->getSubTable(selectedColumn);
            finalTable = Table::getMergedTable(finalTable, tempTable);
        }
        return finalTable;
    }
private:
    // outer vector stores conditions seperated by OR, inner by AND
    vector<vector<shared_ptr<Condition>>> conditions;
    Table* table;
    vector<string> selectedColumns;

    void updateTableRows() const{
        Table* tempTable;
        for(const auto& vec : conditions) {
            // last vector has the result of all ANDs between an OR
            tempTable = vec.at(vec.size()-1)->getConditionedTable();

            for(const auto& rec : tempTable->getTableRecords()) {
                table->addRecord(rec);
            }
        }
    }

    vector<string> getProcessedSelectedColumns() {
        vector<string> processedSelectedColumns;
        processedSelectedColumns.reserve(selectedColumns.size());
        for(const string& column : this->selectedColumns) {
            if(column == "*") {
                for(const string& header : table->getTableHeaders())
                    processedSelectedColumns.push_back(header);
            } else
                processedSelectedColumns.push_back(column);
        }
        return processedSelectedColumns;
    }

    const string getCorrectMatch(const std::smatch& matches) const {
        for(int i = 2; i < matches.length();i++) {
            if(matches[i].length() > 0)
                return matches[i];
        }
        return "-1";
    }
    void setHeaderAndNameFromParameterTable(const Table* t) {
        table = new Table("RESULT");
        for(const string& header : t->getTableHeaders()) {
            table->addHeader(header);
        }
    }

    // creates conditions, only called during object creation
    void init(const Table *t, const string &whereArgs) {
        if(whereArgs.length() == 0) {
            table = new Table(*t);
            return;
        }
        setHeaderAndNameFromParameterTable(t);
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

        updateTableRows();
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
