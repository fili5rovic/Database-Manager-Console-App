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
    QueryEditor(shared_ptr<Database> d) : database(d) {}

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
            } catch (exception& e) {
                cout << e.what() << endl;
                cout << "Press any key to continue..." << endl;
                getch();
            }
        }
    }


private:
    shared_ptr<Database> database;

    const string KEYWORDCOLOR =
            StringManipulator::instance().RGB(184, 51, 106) + StringManipulator::instance().BOLDCOLOR();
    const string  COLOMNCOLOR =
            StringManipulator::instance().RGB(70, 177, 201) + StringManipulator::instance().BOLDCOLOR();
    const string   TABLECOLOR =
            StringManipulator::instance().RGB(66, 255, 186) + StringManipulator::instance().BOLDCOLOR();
    const string   QUOTECOLOR =
            StringManipulator::instance().RGB(238, 108, 77) + StringManipulator::instance().BOLDCOLOR();
    const string   DQUOTECOLOR =
            StringManipulator::instance().RGB(182, 238, 166) + StringManipulator::instance().BOLDCOLOR();
    const string   RESETCOLOR = StringManipulator::instance().RESETCOLOR();


    string colorVectorValuesInString(string str, vector<string> vect, const string& COLOR) const {
        regex pattern;

        // Sort keywords in descending order of length
        std::sort(vect.begin(), vect.end(), [](const string &a, const string &b) {
            return a.length() > b.length();
        });

        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(vect.size());
        for (const string &reg: vect) {
            keywordReplacementStrings.push_back(COLOR + reg + RESETCOLOR);
        }

        for (int i = 0; i < vect.size(); i++) {
            pattern = regex("\\b" + vect[i] + "\\b", regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }
        return str;
    }


    string colorSingleQuotations(const std::string& str) const {
        string result;
        bool openQuot = false;
        for(const char& c : str) {
            if(c == '\'') {
                openQuot = !openQuot;
                if(openQuot)
                    result += QUOTECOLOR;
                else {
                    result += c + RESETCOLOR;
                    continue;
                }
            }
            result += c;
        }
        return result;
    }

    string colorDoubleQuotations(const std::string& str) const {
        string result;
        bool openQuot = false;
        for(const char& c : str) {
            if(c == '\"') {
                openQuot = !openQuot;
                if(openQuot)
                    result += DQUOTECOLOR;
                else {
                    result += c + RESETCOLOR;
                    continue;
                }
            }
            result += c;
        }
        return result;
    }

    string colorQuotations(const std::string& str) const {
        string res = str;
        res = colorDoubleQuotations(res);
        res = colorSingleQuotations(res);
        return res;
    }


    //<editor-fold desc="Clear Console">
    void clearConsole() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    //</editor-fold>


    void printHeaderAndClearConsole() const {
        clearConsole();
        cout << "\033[47m" << "  SQL Query Editor                          " << "\033[41m" << " X "
             << StringManipulator::instance().RESETCOLOR() << endl;
    }

    void paintingConsole(string& lineWithLineFeed) const {
        vector<string> keywords{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "SHOW","TABLES","TABLE", "DROP", "UPDATE" ,"SET", "DELETE"};
        lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, keywords, KEYWORDCOLOR);
        if (regex_search(lineWithLineFeed, regex("from|update", regex_constants::icase)))
            lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, database->getAllTableNames(), TABLECOLOR);
        if (regex_search(lineWithLineFeed, regex("select|where|set", regex_constants::icase)))
            lineWithLineFeed = colorVectorValuesInString(lineWithLineFeed, database->getAllHeaders(), COLOMNCOLOR);
        lineWithLineFeed = colorQuotations(lineWithLineFeed);
    }


//    string listener(int lineCounter) const {
//        int backspace = 8, enter = 13;
//
//        string retStr;
//        char ch;
//        while ((ch = _getch()) != '\r') {
//            retStr += ch;
//            cout << "\r";
//            printLineNum(lineCounter);
//            cout << retStr;
//        }
//        return retStr;
//    }

    vector<string> editor() const {
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

            paintingConsole(lineWithLineFeed);

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
