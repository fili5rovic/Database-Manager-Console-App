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
#include "Insert.h"
#include "Update.h"
#include "Delete.h"

using namespace std;


class QueryEditor {
public:
    QueryEditor(Database *d) : database(d) {}

    QueryEditor(const QueryEditor &) = delete;
    QueryEditor(QueryEditor &&) = delete;

    QueryEditor &operator=(const QueryEditor &) = delete;
    QueryEditor &operator=(QueryEditor &&) = delete;


    void start() {
        vector<string> inputQueries = editor();
        for (auto inputQuery: inputQueries) {
            StringManipulator::removeDuplicateSpaces(inputQuery);
            if (inputQuery.empty())
                return;
            cout << endl << "INPUT QUERY: " << inputQuery << endl;
            try {
                startQuery(inputQuery);
            } catch (MyException &e) {  //TODO catch all exceptions at the end
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

    // todo proper painting
    string colorKeywords(string str) {
        regex pattern;      // add AND, OR later to keywords
        vector<string> keywords{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "TABLE", "DROP"};
        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(keywords.size());
        for (const string &reg: keywords) {
            keywordReplacementStrings.push_back(KEYWORDCOLOR + reg + RESETCOLOR);
        }

        for (int i = 0; i < keywords.size(); i++) {
            pattern = regex(keywords[i], regex_constants::icase);
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


    void printHeaderAndClearConsole() {
        clearConsole();
        cout << "\033[47m" << "  SQL Query Editor                " << "\033[41m" << " X "
             << StringManipulator::instance().RESETCOLOR() << endl;
    }

    vector<string> editor() {
        printHeaderAndClearConsole();
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
            printHeaderAndClearConsole();
            // todo for properly painting split the same line into select, from, where for correct painting :)
            lineWithLineFeed = colorKeywords(lineWithLineFeed);
            if (regex_match(lineWithLineFeed, regex(".*from.*", regex_constants::icase)))
                lineWithLineFeed = colorTables(lineWithLineFeed);
            if (regex_match(lineWithLineFeed, regex(".*(?:select|where).*", regex_constants::icase)))
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
    void startQuery(const string &query) {
//        Statement *type = nullptr;
        std::shared_ptr<Statement> type = nullptr;
        if (regex_match(query, regex("^\\s*select.*", regex_constants::icase))) {
            type = std::make_shared<Select>(query, database);
        } else if (regex_match(query, regex("^\\s*create.*", regex_constants::icase))) {
            type = std::make_shared<Create>(query, database);
        } else if (regex_match(query, regex("^\\s*drop.*", regex_constants::icase))) {
            type = std::make_shared<Drop>(query, database);
        } else if(regex_match(query, regex("^\\s*insert.*",regex_constants::icase))) {
            type = std::make_shared<Insert>(query, database);
        } else if(regex_match(query, regex("^\\s*update.*",regex_constants::icase))) {
            type = std::make_shared<Update>(query, database);
        } else if(regex_match(query, regex("^\\s*delete.*",regex_constants::icase))) {
            type = std::make_shared<Delete>(query, database);
        }else if(regex_match(query, regex("^\\s*show\\s+tables\\s*", regex_constants::icase))) {
            cout << *database;
            return;
        }
        if (!type)
            throw ENoKeywordsException("[SYNTAX_ERROR] No keywords detected. Can't detect query.");

        type->execute();
    }
};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
