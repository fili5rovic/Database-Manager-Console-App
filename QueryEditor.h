#ifndef PRVIPROJEKAT_QUERYEDITOR_H
#define PRVIPROJEKAT_QUERYEDITOR_H


#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include "Database.h"
#include "Select.h"

using namespace std;


class QueryEditor {
public:
    QueryEditor(Database *d) : database(d) {}

    QueryEditor(const QueryEditor &) = delete;

    QueryEditor &operator=(const QueryEditor &) = delete;


    void start() {
        string inputQuery = editor();
        StringManipulator::removeMoreSpaces(inputQuery);

        cout << "INPUT QUERY: " << inputQuery << endl;
        try {
            findOutQueryType(inputQuery);
        } catch(exception& e) {
            cout << StringManipulator::instance().REDCOLOR() << e.what() << StringManipulator::instance().RESETCOLOR() << endl;
            cout << "Press any key to continue..." << endl;
            getch();
        }
    }





private:
    Database *database;

    string colorKeywords(string str) {
        regex pattern;
        vector<string> regexStrings{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "TABLE"};
        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());
        for (const string &reg: regexStrings) {
            keywordReplacementStrings.push_back(StringManipulator::instance().PURPLECOLOR() + reg +
                                                StringManipulator::instance().RESETCOLOR());
        }

        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }
        return str;
    }

    string colorTableColomns(string str) {
        regex pattern;

        vector<string> regexStrings = database->getAllHeaders();
        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());

        // sort from smallest to highest, so the painting works correctly
        std::sort(regexStrings.begin(), regexStrings.end(), [](const string &a, const string &b) {
            return a.length() > b.length();
        });

        // replacements are the same, just added purple text to it.
        for (const auto &columnName: regexStrings) {
            keywordReplacementStrings.push_back(StringManipulator::instance().CYANCOLOR() + columnName +
                                                StringManipulator::instance().RESETCOLOR());
        }

        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }

        return str;
    }


    void deleteConsoleAndPrintHeader() {
        system("cls");
        cout << "\033[47m" << "  SQL Query Editor                " << "\033[41m" << " X " << StringManipulator::instance().RESETCOLOR() << endl;
    }

    string editor() {
        deleteConsoleAndPrintHeader();
        string sqlQuery;
        string line;
        string lineWithLineFeed;
        int lineCounter = 0;
        while (true) {
            cout << StringManipulator::instance().GRAYCOLOR() << to_string(++lineCounter) << StringManipulator::instance().RESETCOLOR() << "  ";
            getline(std::cin, line);

            string lineWithNum = StringManipulator::instance().GRAYCOLOR() + to_string(lineCounter) +
                                 StringManipulator::instance().RESETCOLOR() + "  " + line;

            lineWithLineFeed += (lineWithLineFeed.empty() ? lineWithNum : '\n' + lineWithNum);
            deleteConsoleAndPrintHeader();
            lineWithLineFeed = colorKeywords(lineWithLineFeed);
            lineWithLineFeed = colorTableColomns(lineWithLineFeed);
            cout << lineWithLineFeed << endl;
            if (line.empty())
                break;
            sqlQuery.append(sqlQuery.empty() ? line : (" " + line));
        }

        return sqlQuery;
    }


    // throws Errors, should be caught when being called
    void findOutQueryType(const string& query){
        Statement* type = nullptr;
        if(regex_match(query,regex("^\\s*select.*", regex_constants::icase))) {
            cout << "User wanted select query" << endl;
            type = new Select(query);
        }
        if(!type) {
            throw ENoKeywordsException("[ERROR] No keywords detected. Can't detect query.");
        }
        type->init();
    }





};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
