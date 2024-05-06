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
    Select(const string &input,shared_ptr<Database> database) : Statement(input, database){}

private:
    void executingQuery(const smatch &matches) const override {
        string argumentsStr = matches[1];
        StringManipulator::removeSpaces(argumentsStr);
        string fromTableStr = matches[2];
        string whereStr = matches[3];

        // todo select nesto.nestodrugo

        unordered_map<string,string> tableAcronymMap;
        std::smatch matchForElse;
        if(regex_search(fromTableStr, regex("\\w+(?:\\s+\\w+)?((?:\\s+inner)?\\s+join\\s+\\w+(?:\\s+\\w+)?\\s+on\\s+\\w+\\.\\w+\\s*=\\s*\\w+\\.\\w+)+", regex_constants::icase))) {

            fromTableStr = regex_replace(fromTableStr, regex("\\s*INNER\\s*",regex_constants::icase), " ");
            fromTableStr = regex_replace(fromTableStr, regex("\\s*JOIN\\s*",regex_constants::icase), "|");


            vector<string> subStrings = StringManipulator::instance().splitString(fromTableStr,'|');



            std::smatch innerMatches;
            if(regex_match(subStrings[0],innerMatches,regex("(\\w+)(?:\\s+(\\w+))?",regex_constants::icase))) {
                string tableName = innerMatches[1];
                string tableNick = innerMatches[2];

                cout << "Table name:" << tableName << endl;
                cout << "Table nick:" << tableNick << endl;

                if(database->tryGettingTableByNameCaseI(tableName) == nullptr) {
                    throw EBadArgumentsException("[RUNTIME_ERROR] Table '"+ tableName +"' not found.");
                }

                tableAcronymMap[tableName] = tableNick;

            } else throw EBadArgumentsException("[ERROR] Bad Syntax around: " + subStrings[0]);

            for(auto it = subStrings.begin() + 1; it != subStrings.end(); ++it) {
                if(regex_search(*it,innerMatches,regex("(\\w+)(?:\\s+(\\w+))?\\s+on\\s+(\\w+)\\.(\\w+)\\s*(\\=|\\!\\=)\\s*(\\w+)\\.(\\w+)",regex_constants::icase))) {
                    string tableName = innerMatches[1];
                    string tableNick = innerMatches[2];
                    string firstTableNickInJoin = innerMatches[3];
                    string firstTableColumnNameInJoin = innerMatches[4];
                    string equasion = innerMatches[5];
                    string secondTableNickInJoin = innerMatches[6];
                    string secondTableColumnNameInJoin = innerMatches[7];

                    if(database->tryGettingTableByNameCaseI(tableName) == nullptr) {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Table '"+ tableName +"' not found.");
                    }

                    if (tableAcronymMap.find(tableName) == tableAcronymMap.end()) {
                        tableAcronymMap[tableName] = tableNick;
                    }
                    else {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Same table acronyms not allowed.");
                    }

                    string table1Name = getTableNameFromAcronym(tableAcronymMap, firstTableNickInJoin);
                    string table2Name = getTableNameFromAcronym(tableAcronymMap, secondTableNickInJoin);

                    shared_ptr<Table> table1 = database->tryGettingTableByNameCaseI(table1Name);
                    shared_ptr<Table> table2 = database->tryGettingTableByNameCaseI(table2Name);

                    if(table1 == nullptr) {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Table '"+ table1Name +"' not found.");
                    }
                    if(table2 == nullptr) {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Table '"+ table2Name +"' not found.");
                    }

                    int column1Index = 0;
                    int column2Index = 0;


                    bool firstTableColumnExists = false;
                    bool secondTableColumnExists = false;

                    for (int i = 0; i < table1->getTableHeaders().size(); ++i) {
                        if (regex_match(table1->getTableHeaders().at(i), regex(firstTableColumnNameInJoin, regex_constants::icase))) {
                            column1Index = i;
                            firstTableColumnExists = true;
                        }
                    }
                    for (int i = 0; i < table2->getTableHeaders().size(); ++i) {
                        if (regex_match(table2->getTableHeaders().at(i), regex(secondTableColumnNameInJoin, regex_constants::icase))) {
                            column2Index = i;
                            secondTableColumnExists = true;
                        }
                    }

                    if (!firstTableColumnExists) {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Column name '" + firstTableColumnNameInJoin + "' not found in " + table1->getName());
                    } else if (!secondTableColumnExists) {
                        throw EBadArgumentsException("[RUNTIME_ERROR] Column name '" + secondTableColumnNameInJoin + "' not found in " + table2->getName());
                    }

                    string filterString;
                    for (int i = 0; i < table1->getTableRecords().size(); ++i) {
                        string str = secondTableColumnNameInJoin + " = \"" + table1->getTableRecords().at(i).getData().at(column1Index) + "\"";
                        filterString = filterString.empty() ? str : filterString + " OR " + str;
                    }

                    cout << "FILTER STRING: " << filterString << endl;
                    shared_ptr<Table> filTable;
                    try {
                        Filter fil = Filter(table2, filterString);
                        filTable = fil.getFilteredTable();
                    } catch( MyException& e) {
                        cout << e.what() << endl;
                    }
                    // dodaj acronym ispred svakog headera pre nego sto ih spojis

                    cout << *Table::getMergedTablesForJoin(table1, filTable);
                    // todo this->table = ovo od iznad

                } else // todo add USING regex else if here
                    throw EBadArgumentsException("[SYNTAX_ERROR] Bad Syntax around: " + *it);
            }
        } else if(regex_match(fromTableStr, matchForElse,regex("^\\s*(\\w+)(?:\\s+(\\w+))?\\s*$",regex_constants::icase))) {
            string tableName = matchForElse[1];
            string tableNick = matchForElse[2];

            if(database->tryGettingTableByNameCaseI(tableName) == nullptr) {
                throw EBadArgumentsException("[RUNTIME_ERROR] Table \'"+ tableName +"\' not found.");
            }

            tableAcronymMap[tableName] = tableNick;
        }

        vector<string> selectedColumns = StringManipulator::splitString(argumentsStr, ',');


        shared_ptr<Table> filteredTable;

        try {
            Filter f(this->table,whereStr);
            filteredTable = f.getFilteredTable();
        } catch(EInvalidColumnNameException& e) {
            cout << e.what() << endl;
        }

        cout << *getTableWithSelectedColumns(filteredTable,selectedColumns);
    }

    string getTableNameFromAcronym(const unordered_map<string,string> tableNickNames, const string value) const {
        for (const auto& pair : tableNickNames) {
            if (pair.second == value) {
                return pair.first;
            }
        }
        throw EBadArgumentsException("[RUNTIME_ERROR] Table acronym '"+ value +"' not found.");
    }



    static shared_ptr<Table> getTableWithSelectedColumns(shared_ptr<Table> tableArg, vector<string> selectedColumns)  {
        if(selectedColumns.size() == 1 && selectedColumns.at(0) == "*") {
            return tableArg;
        }

        shared_ptr<Table> finalTable = make_shared<Table>(tableArg->getName());
        selectedColumns = getProcessedSelectedColumns(tableArg, selectedColumns);

        for(int i = 0; i < tableArg->getTableRecords().size(); i++)
            finalTable->addRecord(Record());

        for(const auto& selectedColumn : selectedColumns) {
            shared_ptr<Table> tempTable = tableArg->getSubTable(selectedColumn);
            finalTable = Table::getMergedTable(finalTable, tempTable);
        }
        return finalTable;
    }

    static vector<string> getProcessedSelectedColumns(shared_ptr<Table> tableArg, vector<string> selectedColumns) {
        vector<string> processedSelectedColumns;
        processedSelectedColumns.reserve(selectedColumns.size());
        for(const string& column : selectedColumns) {
            if(column == "*") {
                for(const string& header : tableArg->getTableHeaders())
                    processedSelectedColumns.push_back(header);
            } else
                processedSelectedColumns.push_back(column);
        }
        return processedSelectedColumns;
    }


    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override {
        // FIXME
        throw EBadArgumentsException("[RUNTIME_ERROR] Runtime error.");
    }

    void checkForSyntaxErrors(const string &query) const {
        if (regex_match(query, regex("^\\s*select(?:\\s+from.*|\\s*)", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] SELECT has no arguments.");
        } else if (!regex_match(query, regex(".*\\s+from\\s*.*", regex_constants::icase))) {
            throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
        } // table name not detected
        else if (!regex_search(query, regex(".*\\s+from\\s+\\w+\\s*", regex_constants::icase))) {
            throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
        }
        // multiple keywords
        if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with SELECT not allowed.");
        } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with SELECT not allowed.");
        } else if (regex_match(query, regex(".*\\bdelete\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] DELETE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with SELECT not allowed.");
        } else if (regex_match(query, regex(".*\\bselect\\b.*\\bselect\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple SELECT keywords not allowed.");
        } else if (regex_match(query, regex(".*\\bfrom\\b.*\\bfrom\\b.*", regex_constants::icase))) {
            throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple FROM keywords not allowed.");
        }


    }
    //</editor-fold>


    //<editor-fold desc="Getters">
    regex getRegexPattern() const {
        return regex(R"(^\s*select\s+((?:(?:\w+\.)?\w+|\*)(?:\s*,\s*(?:(?:\w+\.)?\w+|\*))*)\s+from\s+(\w+\s+(?:\w+\s+)?(?:inner\s+)?join\s+\w+\s+(?:\w+\s+)?on\s+\w+\.\w+\s*\=\s*\w+\.\w+|\w+)\s*(?:where\s+((\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+)(?:\s+(and|or)\s*(\w+)\s*(\=|\<\>|\!\=)\s*('\w+'|"\w+"|\w+))*))?)", regex_constants::icase);;
    }

    regex getRegexForFindingTable() const override {
        return regex("from\\s+(\\w+)", regex_constants::icase);
    }
    //</editor-fold>


};


#endif //PRVIPROJEKAT_SELECT_H
