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
            } catch (exception& e) {
                cout << e.what() << endl;
                cout << "Press any key to continue..." << endl;
                getch();
            }
        }
    }


private:
    Database *database;

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

    // todo proper painting
    string colorKeywords(string str) const {
        regex pattern;      // add AND, OR later to keywords
        vector<string> keywords{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "TABLE", "DROP","UPDATE" ,"SET", "DELETE"};
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


    string colorTableColumns(string str) const {
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

    string colorTables(string str) const {
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

    void clearConsole() const {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }


    void printHeaderAndClearConsole() const{
        clearConsole();
        cout << "\033[47m" << "  SQL Query Editor                          " << "\033[41m" << " X "
             << StringManipulator::instance().RESETCOLOR() << endl;
    }

    void paintingConsole(string& lineWithLineFeed) const {
        lineWithLineFeed = colorKeywords(lineWithLineFeed);
        if (regex_match(lineWithLineFeed, regex(".*from.*|.*update.*", regex_constants::icase)))
            lineWithLineFeed = colorTables(lineWithLineFeed);
        if (regex_match(lineWithLineFeed, regex(".*(?:select|where|set).*", regex_constants::icase)))
            lineWithLineFeed = colorTableColumns(lineWithLineFeed);
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
