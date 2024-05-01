#ifndef PRVIPROJEKAT_QUERYEDITOR_H
#define PRVIPROJEKAT_QUERYEDITOR_H


#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "Database.h"
#include "Select.h"
#include "Create.h"
#include "Drop.h"

using namespace std;


class QueryEditor {
public:
    QueryEditor(Database *d) : database(d) {}

    QueryEditor(const QueryEditor &) = delete;

    QueryEditor &operator=(const QueryEditor &) = delete;


    void start() {
        vector<string> inputQueries = editor();
        for (auto inputQuery: inputQueries) {
            StringManipulator::removeMoreSpaces(inputQuery);
            if(inputQuery.empty())
                return;
            cout << endl << "INPUT QUERY: " << inputQuery << endl;
            try {
                findOutQueryType(inputQuery);
            } catch (exception &e) {
                cout << StringManipulator::instance().REDCOLOR() << e.what()
                     << StringManipulator::instance().RESETCOLOR() << endl;
                cout << "Press any key to continue..." << endl;
                getch();
            }
        }
    }


private:
    Database *database;

    const string KEYWORDCOLOR = StringManipulator::instance().MYCOLOR(255, 60, 243);
    const string COLOMNCOLOR = StringManipulator::instance().CYANCOLOR();
    const string TABLECOLOR = StringManipulator::instance().MYCOLOR(66, 255, 186);
    const string RESETCOLOR = StringManipulator::instance().RESETCOLOR();


    string colorKeywords(string str) {
        regex pattern;
        vector<string> regexStrings{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "TABLE"};
        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());
        for (const string &reg: regexStrings) {
            keywordReplacementStrings.push_back(KEYWORDCOLOR + reg + RESETCOLOR);
        }

        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }
        return str;
    }

    string colorTableColumns(string str) {
        regex pattern;

        vector<string> regexStrings = database->getAllHeaders();

        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());

        // sort from smallest to highest, so the painting works correctly
        std::sort(regexStrings.begin(), regexStrings.end(), [](const string &a, const string &b) {
            return a.length() > b.length();
        });


        // replacements are the same, just added color
        for (const auto &columnName: regexStrings) {
            keywordReplacementStrings.push_back(COLOMNCOLOR + columnName + RESETCOLOR);
        }


        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }

        return str;
    }

    string colorTables(string str) {
        regex pattern;

        vector<string> regexStrings = database->getAllTableNames();

        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());

        // sort from smallest to highest, so the painting works correctly
        std::sort(regexStrings.begin(), regexStrings.end(), [](const string &a, const string &b) {
            return a.length() > b.length();
        });

        // replacements are the same, just added table color to it.
        for (const auto &columnName: regexStrings) {
            keywordReplacementStrings.push_back(TABLECOLOR + columnName + RESETCOLOR);
        }


        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }

        return str;
    }

    void clearConsole() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }


    void deleteConsoleAndPrintHeader() {
        clearConsole();
        cout << "\033[47m" << "  SQL Query Editor                " << "\033[41m" << " X "
             << StringManipulator::instance().RESETCOLOR() << endl;
    }

    vector<string> editor() {
        deleteConsoleAndPrintHeader();
        string sqlQuery;
        string line;
        string lineWithLineFeed;
        int lineCounter = 0;
        while (true) {
            cout << StringManipulator::instance().GRAYCOLOR() << to_string(++lineCounter)
                 << StringManipulator::instance().RESETCOLOR() << "  ";
            getline(std::cin, line);

            string lineWithNum = StringManipulator::instance().GRAYCOLOR() + to_string(lineCounter) +
                                 StringManipulator::instance().RESETCOLOR() + "  " + line;

            lineWithLineFeed += (lineWithLineFeed.empty() ? lineWithNum : '\n' + lineWithNum);
            deleteConsoleAndPrintHeader();
            // todo for properly painting split the same line into select, from, where for correct painting :)
            lineWithLineFeed = colorKeywords(lineWithLineFeed);
            if(regex_match(lineWithLineFeed,regex(".*from.*",regex_constants::icase)))
                lineWithLineFeed = colorTables(lineWithLineFeed);
            if(regex_match(lineWithLineFeed,regex(".*(?:select|where).*",regex_constants::icase)))
                lineWithLineFeed = colorTableColumns(lineWithLineFeed);
            cout << lineWithLineFeed << endl;
            if (line.empty())
                break;
            sqlQuery.append(sqlQuery.empty() ? line : (" " + line));
        }

        vector<string> sqlQueries = StringManipulator::splitString(sqlQuery, ';');
        return sqlQueries;
    }


    // throws Errors, should be caught when being called
    void findOutQueryType(const string &query) {
        Statement *type = nullptr;
        if (regex_match(query, regex("^\\s*select.*", regex_constants::icase))) {
            // from not detected
            if (!regex_match(query, regex(".*\\s+from\\s+.*", regex_constants::icase))) {
                throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
            } // table name not detected
            else if (!regex_search(query, regex(".*\\s+from\\s+\\w+\\s*", regex_constants::icase))) {
                throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
            }

            Table *tableForSelect = tryToGetTableFromSelectQuery(query);

            type = new Select(query, tableForSelect);
        } else if (regex_match(query, regex("^\\s*create.*", regex_constants::icase))) {

            type = new Create(query, database);

        } else if (regex_match(query, regex("^\\s*drop.*", regex_constants::icase))) {
            cout << "DROP" << endl;
//            type = new Drop(tabl);
        }
        if (!type) {
            throw ENoKeywordsException("[SYNTAX_ERROR] No keywords detected. Can't detect query.");
        }
        type->execute();
    }


    const string getTableNameForErrorMsg(const string &query) {
        std::smatch matches;
        if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            return matches[1];
        }
        return "";
    }

    Table *tryToGetTableFromSelectQuery(const string &query) {
        Table *table = nullptr;
        vector<string> names;

        std::smatch matches;
        // without join
        if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            return database->tryGettingTableByNameCaseI(matches[1]);
        }
        // with join on
        else if (regex_search(query, matches,
                              regex("from\\s+(\\w+)\\s+(\\w+)\\s+(?:(?:inner\\s+)?join\\s+(\\w+)\\s+(\\w+)\\s+(?:on\\s+(?:\\w+\\.\\w+\\s*\\=\\s*\\w+\\.\\w+\\s*)))+",
                                    regex_constants::icase))) {
            // make a new table that's joined
        }
        // with join using
        else if (regex_search(query, matches, regex("from\\s+(\\w+)", regex_constants::icase))) {
            // make a new table that's joined
        }

        if (!table)
            throw EBadArgumentsException("[RUNTIME_ERROR] Table " + getTableNameForErrorMsg(query) + " not found.");
        return table;
    }




};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
