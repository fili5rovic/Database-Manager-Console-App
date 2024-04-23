#ifndef PRVIPROJEKAT_QUERYEDITOR_H
#define PRVIPROJEKAT_QUERYEDITOR_H


#include <iostream>
#include <string>
#include <regex>
#include <conio.h>

using namespace std;


class QueryEditor {
public:
    // Static member function to get the singleton instance
    static QueryEditor &getInstance() {
        static QueryEditor instance;
        return instance;
    }

    QueryEditor(const QueryEditor &) = delete;

    QueryEditor &operator=(const QueryEditor &) = delete;

    void start() {
        deleteConsoleAndPrintHeader();
        editor();
    }

private:
    QueryEditor() {}

    ~QueryEditor() = default;


    const string PURPLE = "\033[35m";
    const string RESET = "\033[0m";
    const string GRAY = "\033[37m";
    const string RedBG = "\033[41m";
    const string GreyBG = "\033[47m";

    string colorKeywords(string str) {
        regex pattern;
        vector<string> regexStrings{"select", "from", "where"};
        vector<string> keywordReplacementStrings = vector<string>{PURPLE + "SELECT" + RESET,
                                                                  PURPLE + "FROM" + RESET,
                                                                  PURPLE + "WHERE" + RESET};
        for (int i = 0; i < regexStrings.size(); i++) {
            pattern = regex(regexStrings[i], regex_constants::icase);
            str = regex_replace(str, pattern, keywordReplacementStrings[i]);
        }
        return str;
    }


    void deleteConsoleAndPrintHeader() {
        system("cls");
        cout << GreyBG + "  SQL Query Editor                " << RedBG << " X " << RESET << endl;
    }


    void editor() {
        string sqlQuery;
        string line;

        string lineWithLineFeed;

        int lineCounter = 0;
        while (true) {
            cout << GRAY << to_string(++lineCounter) << RESET << "  ";
            getline(std::cin, line);
//            line += getchar();

            string lineWithNum = GRAY + to_string(lineCounter) + RESET + "  " + line;

            lineWithLineFeed += (lineWithLineFeed.empty() ? lineWithNum : '\n' + lineWithNum);
            deleteConsoleAndPrintHeader();
            cout << colorKeywords(lineWithLineFeed) << endl;
            if (line.empty())
                break;
            sqlQuery.append(sqlQuery.empty() ? line : (" " + line) );
        }

        cout << "QUERY: " << sqlQuery << endl;
    }

};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
