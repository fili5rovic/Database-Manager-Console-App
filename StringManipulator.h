#ifndef PRVIPROJEKAT_STRINGMANIPULATOR_H
#define PRVIPROJEKAT_STRINGMANIPULATOR_H

#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include "Table.h"

using namespace std;

class StringManipulator {
private:
    const char topLeftCorner = '\xDA';
    const char bottomLeftCorner = '\xC0';
    const char topRightCorner = '\xBF';
    const char bottomRightCorner = '\xD9';
    const char middleLeftCorner = '\xC3';
    const char middleRightCorner = '\xB4';
    const char horizontalLine = '\xC4';
    const char verticalLine = '\xB3';
    const char intersection = '\xC5';
    const char middleDown = '\xC2';
    const char middleUp = '\xC1';
    const string RED = "\033[38;2;255;0;0m";
    const string GREEN = "\033[32m";
    const string GRAY = "\033[37m";
    const string CYAN = "\033[36m";
    const string PURPLE = "\033[35m";
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
public:

    static StringManipulator instance() {
        static StringManipulator instance;
        return instance;
    }

    string MYCOLOR(int r, int g=0, int b=0) {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
    }

    const string &REDCOLOR() const {
        return RED;
    }

    const string &GREENCOLOR() const {
        return GREEN;
    }

    const string &GRAYCOLOR() const {
        return GRAY;
    }
    const string &CYANCOLOR() const {
        return CYAN;
    }
    const string &PURPLECOLOR() const {
        return PURPLE;
    }

    const string &RESETCOLOR() const {
        return RESET;
    }

    const string &BOLDCOLOR() const {
        return BOLD;
    }

    string printline(int n) {
        string s;
        for (int i = 0; i < n; ++i) {
            s += '\xC4';
        }
        return s;
    }

    string printspace(int n) {
        string s;
        for (int i = 0; i < n; ++i) {
            s += ' ';
        }
        return s;
    }

    void upperTableHeading(int n, bool closing = true, bool end = true) {
        cout << topLeftCorner << printline(n);
        if (closing) cout << topRightCorner;
        if (end) cout << endl;
    }

    void lowerTableHeading(int n, bool closing = true, bool end = true) {
        cout << bottomLeftCorner << printline(n);
        if (closing) cout << bottomRightCorner;
        if (end) cout << endl;
    }

    void newMenu(int line_len, int strlen, string options[], int pomeraj = 7) {
        for (int i = 0; i < strlen; ++i) {
            if (options[i].length() % 2 != 0)
                options[i] += " ";
        }

        cout << endl;
        upperTableHeading(line_len);
        cout << verticalLine << printspace((line_len - options[0].length()) / 2) << options[0]
             << printspace((line_len - options[0].length()) / 2) << verticalLine << endl;
        cout << middleLeftCorner << printline(line_len) << middleRightCorner << endl;
        for (int i = 1; i < strlen; ++i) {
            cout << verticalLine << printspace(pomeraj) << options[i]
                 << printspace((line_len - options[i].length()) - pomeraj) << verticalLine << endl;
            if (i < strlen - 1)
                cout << middleLeftCorner << printline(line_len) << middleRightCorner << endl;
        }
        lowerTableHeading(line_len);
    }

    void newMessage(string poruka, int line_len = -1) {
        if (line_len == -1)
            line_len = poruka.length() + 6;
        else if (poruka.length() % 2 != 0)
            poruka += " ";
        cout << endl;
        upperTableHeading(line_len);
        cout << verticalLine << printspace((line_len - poruka.length()) / 2) << poruka
             << printspace((line_len - poruka.length()) / 2) << verticalLine << endl;
        lowerTableHeading(line_len);
    }

    void newMessageGreen(string poruka, int line_len = -1) {
        cout << GREEN;
        newMessage(std::move(poruka), line_len);
        cout << RESET;
    }

    void newMessageRed(string poruka = "", int line_len = -1) {
        cout << RED;
        newMessage(std::move(poruka), line_len);
        cout << RESET;
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

    static void removeDuplicateSpaces(string &str) {
        str.erase(std::unique(str.begin(), str.end(), [](char a, char b) {
            return a == ' ' && b == ' ';
        }), str.end());
    }

    static void removeSpaces(string& str) {
        string result;
        for (char c : str) {
            if (c != ' ') {
                result += c;
            }
        }
        str = result;
    }


    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == string::npos)
            return "";

        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }


    void removeQuotesAndLeadingSpaces(vector<string>& strings) {
        vector<string> result;
        for (string& str : strings) {
            str = trim(str);
            if ((str.front() == '\"' && str.back() == '\"') || (str.front() == '\'' && str.back() == '\''))
                str = str.substr(1, str.size() - 2);
            result.push_back(str);
        }
    }



};


#endif //PRVIPROJEKAT_STRINGMANIPULATOR_H
