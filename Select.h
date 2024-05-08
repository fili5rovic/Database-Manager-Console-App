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
        shared_ptr<Table> selectTable = this->table;

        string argumentsStr = matches[1];
        StringManipulator::removeSpaces(argumentsStr);
        string fromTableStr = matches[2];
        string whereStr = matches[3];

        unordered_map<string,string> tableAcronymMap;
        std::smatch matchForElse;
        if(regex_search(fromTableStr, regex("\\w+(?:\\s+\\w+)?((?:\\s+inner)?\\s+join\\s+\\w+(?:\\s+\\w+)?\\s+on\\s+\\w+\\.\\w+\\s*=\\s*\\w+\\.\\w+)+", regex_constants::icase))) {
            cout << "JOIN" << endl;
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

            vector<shared_ptr<Table>> mergedTables;

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
                        cout << *table1;
                        throw EBadArgumentsException("[RUNTIME_ERROR] Column name '" + firstTableColumnNameInJoin + "' not found in " + table1->getName());
                    } else if (!secondTableColumnExists) {
                        cout << *table2;
                        throw EBadArgumentsException("[RUNTIME_ERROR] Column name '" + secondTableColumnNameInJoin + "' not found in " + table2->getName());
                    }

                    string filterString;
                    for (int i = 0; i < table1->getTableRecords().size(); ++i) {
                        string str = secondTableColumnNameInJoin + " = \"" + table1->getTableRecords().at(i).getData().at(column1Index) + "\"";
                        filterString = filterString.empty() ? str : filterString + " OR " + str;
                    }

                    shared_ptr<Table> filTable;
                    Filter fil = Filter(table2, filterString);
                    filTable = fil.getFilteredTable();

//                    selectTable = Table::getMergedTablesForJoin(table1, filTable);
//                    cout << *selectTable;

                    table1->addAcronymToTableHeader(firstTableNickInJoin);// ovo si poslednje radio
                    filTable->addAcronymToTableHeader(secondTableNickInJoin);


                    if(mergedTables.empty())
                        mergedTables.push_back(table1);
                    mergedTables.push_back(filTable);

//                    table1->removeAcronymFromTableHeader();//   ovo si poslednje radio
//                    filTable->removeAcronymFromTableHeader();


                    // mozda mozes i hesh mapu da koristis koja ima tabele pa kada napise aa.ID da dohvatis iz heshmape aa. Mozes onda filter da primenis na svaku zasebno tabelu umesto ovako
                    // a da ih samo ispisujes zajedno kao jednu tabelu

                } else
                    throw EBadArgumentsException("[SYNTAX_ERROR] Bad Syntax around: " + *it);
            }

            shared_ptr<Table> finalTable = mergedTables[0];
            for (int i = 1; i < mergedTables.size(); i++) {
                finalTable = Table::getMergedTablesForJoin(finalTable, mergedTables[i]);
            }
            selectTable = finalTable;
            cout << *selectTable;

            shared_ptr<Table> filteredTable;
            Filter f(selectTable,whereStr);
            filteredTable = f.getFilteredTable();

            vector<string> selectedColumns = StringManipulator::splitString(argumentsStr, ',');
            cout << *getTableWithSelectedColumns(filteredTable,selectedColumns);
            selectTable->removeAcronymFromTableHeader();
            return;

        }
        else if(regex_match(fromTableStr, matchForElse,regex("^\\s*(\\w+)(?:\\s+(\\w+))\\s*$",regex_constants::icase))) {
            cout << "FROM (A) AA " << endl;
            string tableName = selectTable->getName();
            string tableNick = matchForElse[2];
            cout << "TABLE NICK: " << tableNick << endl;

            tableAcronymMap[tableName] = tableNick;

            selectTable->addAcronymToTableHeader(tableNick);

            for(const string& header : selectTable->getTableHeaders()) {
                cout << "HEADER: " << header << endl;
            }

        }

        vector<string> selectedColumns = StringManipulator::splitString(argumentsStr, ',');

        for(string& column : selectedColumns) {
            cout << "SELECTED: " << column << endl;
            if(regex_match(column,regex("\\w+\\.\\w+",regex_constants::icase))) {
                cout << "SA TACKOM" << endl;
                vector<string> splitColumn = StringManipulator::splitString(column,'.');
                string tableNick = splitColumn[0];
                string columnName = splitColumn[1];

                string tableName = getTableNameFromAcronym(tableAcronymMap, tableNick);

                shared_ptr<Table> tableFromAcronym = database->tryGettingTableByNameCaseI(tableName);

                if(tableFromAcronym == nullptr) {
                    selectTable->removeAcronymFromTableHeader();
                    throw EBadArgumentsException("[RUNTIME_ERROR] Table \'"+ tableName +"\' not found.");
                } else if (tableFromAcronym != selectTable) {
                    selectTable->removeAcronymFromTableHeader();
                    throw EBadArgumentsException("[RUNTIME_ERROR] Invalid acronym \'" + tableNick + "\' in SELECT.");
                }

                bool columnExists = false;
                for (const string& header : selectTable->getTableHeaders()) {
                    cout << "HEADER: " << header << " COLUMN " << column << endl;
                    if (regex_match(header, regex(column, regex_constants::icase))) {
                        cout << "FOUND1:" << header << endl;

                        // everything works except when you put table name in front of where clause
//                        select *
//                        from a aa
//                        where a.id=5
                        // just ignore acronym if it's a table name


                        whereStr = regex_replace(whereStr, std::regex("\\b" + columnName + "\\b"), column);
                        whereStr = std::regex_replace(whereStr, std::regex("\\b" + tableNick + "\\." + tableNick + "\\b"), tableNick);
                        cout << "WHERESTRING:" << whereStr << endl;
                        columnExists = true;
                        break;
                    }
                }
                if(!columnExists) {
                    // for table name A.ID for example
                    for (const string& header : selectTable->getTableHeaders()) {
                        cout << "HEADER: " << header << " COLUMN " << column << endl;
                        if (regex_match(column, regex("\\w+\\."+header, regex_constants::icase))) {
                            cout << "FOUND2:" << header << " COLUMN:" << column << endl;
                            column = header;
                            columnExists = true;
                            break;
                        }
                    }
                }
                if (!columnExists) {
                    selectTable->removeAcronymFromTableHeader();
                    throw EBadArgumentsException("[RUNTIME_ERROR] Column name \'" + columnName + "\' not found in " + tableFromAcronym->getName());
                }
            }
            else {
                cout << "bez tacke" << endl;
                if(column != "*")
                    checkForAmbiguity(column);
                // nakon ove naredbe znam da necu imati dve identicne kolone unutar tabele

                if(!tableAcronymMap.empty()) {
                    for(const auto& pair : tableAcronymMap) {
                        string tableName = pair.first;
                        string tableNick = pair.second;

                        shared_ptr<Table> tableFromAcronym = database->tryGettingTableByNameCaseI(tableName);
                        for(const string& header : tableFromAcronym->getTableHeaders()) {
                            if(regex_match(header,regex("\\w+\\." + column,regex_constants::icase))) {
                                column = header;
                                break;
                            }
                        }
                    }
                } else {
                    cout << "NORMAL" << endl;
                }
            }
        }

        for(const string& column : selectedColumns) {
            cout << "SELECTED COL: " << column << endl;
        }

        shared_ptr<Table> filteredTable;

//        cout << *selectTable;

        try {
            Filter f(selectTable,whereStr);
            filteredTable = f.getFilteredTable();
        } catch(EInvalidColumnNameException& e) {
            cout << e.what() << endl;
        }

        cout << *getTableWithSelectedColumns(filteredTable,selectedColumns);
        selectTable->removeAcronymFromTableHeader();
    }

    void checkForAmbiguity(const string& column) const {
        int cnt = 0;
        for(const string& tableColumn : this->table->getTableHeaders()) {
            vector<string> splitColumn = StringManipulator::instance().splitString(tableColumn,'.');
            string columnCheck = (splitColumn.size()==1 ? splitColumn[0] : splitColumn[1]);

            if(regex_match(columnCheck,regex(column,regex_constants::icase))) {
                cnt++;
            }
        }
        if(cnt > 1)
            throw EBadArgumentsException("[RUNTIME_ERROR] Ambiguous column name \'" + column + "\'.");
    }

    // todo check with full table name
    string getTableNameFromAcronym(const unordered_map<string,string>& tableNickNames, const string value) const {
        for (const auto& pair : tableNickNames) {
            cout << "VAL: " << pair.second << endl;
            if (pair.second == value) {
                return pair.first;
            }
        }

        for(const auto& a : database->getAllTableNames()) {
            if(regex_match(a,regex(value,regex_constants::icase)))
                return a;
        }

        throw EBadArgumentsException("[RUNTIME_ERROR] Table acronym '"+ value +"' missing.");
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
        return regex("^\\s*select\\s+((?:(?:\\w+\\.)?\\w+|\\*)(?:\\s*,\\s*(?:(?:\\w+\\.)?\\w+|\\*))*)\\s+from\\s+(\\w+(?:\\s+[^(?:where)])?(?:\\s+(?:inner\\s+)?join\\s+\\w+\\s+(?:\\w+\\s+)?on\\s+\\w+\\.\\w+\\s*\\=\\s*\\w+\\.\\w+|\\w+(?:\\s+\\w+)?)*\\s*)\\s*(?:where\\s+(((?:\\w+\\.)?\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*((?:\\w+\\.)?\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?", regex_constants::icase);;
    }

    regex getRegexForFindingTable() const override {
        return regex("from\\s+(\\w+)", regex_constants::icase);
    }
    //</editor-fold>


};


#endif //PRVIPROJEKAT_SELECT_H
