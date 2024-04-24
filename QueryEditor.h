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
        removeMoreSpaces(inputQuery);

        cout << "INPUT QUERY: " << inputQuery << endl;

        findOutQueryType(inputQuery);
    }



    static vector<string> splitString(const string &input, const char &delim = ' ') {
        vector<string> tokens;
        istringstream iss(input);
        string token;
        while (getline(iss, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

private:
    Database *database;

    const string PURPLE = "\033[35m";
    const string CYAN = "\033[36m";
    const string RESET = "\033[0m";
    const string GRAY = "\033[37m";
    const string RedBG = "\033[41m";
    const string GreyBG = "\033[47m";

    string colorKeywords(string str) {
        regex pattern;
        vector<string> regexStrings{"SELECT", "FROM", "WHERE", "INSERT", "INTO", "CREATE", "TABLE"};
        vector<string> keywordReplacementStrings;
        keywordReplacementStrings.reserve(regexStrings.size());
        for (const string &reg: regexStrings) {
            keywordReplacementStrings.push_back(PURPLE + reg + RESET);
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
            keywordReplacementStrings.push_back(CYAN + columnName + RESET);
        }

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

    string editor() {
        deleteConsoleAndPrintHeader();
        string sqlQuery;
        string line;
        string lineWithLineFeed;
        int lineCounter = 0;
        while (true) {
            cout << GRAY << to_string(++lineCounter) << RESET << "  ";
            getline(std::cin, line);

            string lineWithNum = GRAY + to_string(lineCounter) + RESET + "  " + line;

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

    void removeMoreSpaces(string &str) {
        str.erase(std::unique(str.begin(), str.end(), [](char a, char b) {
            return a == ' ' && b == ' ';
        }), str.end());
    }

    void removeSpaces(string& str) {
        string result;
        for (char c : str) {
            if (c != ' ') {
                result += c;
            }
        }
        str = result;
    }

    void findOutQueryType(const string& query) {
        Statement* type;
        if(regex_match(query,regex("^\\s*select.*", regex_constants::icase))) {
            cout << " Korisnik je hteo select upit" << endl;

            type = new Select(query);
        }
    }




    void handleQuery(const string& inputQuery) {
        std::smatch matches;
        if(regex_match(inputQuery,Select::getRegexPattern())) {
            cout << "SELECT UPIT!" << endl;
        }
        if(regex_search(inputQuery,matches,Select::getRegexPattern())) {
            cout << "SELECT UPIT yes" << endl;
            cout << "Number of groups: " << matches.size() - 1 << endl; // Exclude the whole match
            for (size_t i = 1; i < matches.size(); ++i) {
                cout << "Group " << i << ": " << matches[i] << endl;
            }


            string argumentsStr = matches[1];
            removeSpaces(argumentsStr);
            string fromTable = matches[3];

            vector<string> arguments = splitString(argumentsStr, ',');
            for(const auto& str:arguments) {
                cout << str << endl;
            }

            cout << "Table: " << fromTable << endl;
        }

    }
};


#endif //PRVIPROJEKAT_QUERYEDITOR_H
