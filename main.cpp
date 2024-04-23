#include <iostream>
#include <utility>
#include <sstream>
#include "Database.h"
#include "QueryEditor.h"
#include <vector>

using namespace std;


//<editor-fold desc="Ispisi">
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
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string GRAY = "\033[37m";
const string RESET = "\033[0m";
const string BOLD = "\033[1m";

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

//</editor-fold>

vector<string> splitString(const string &input, const char &delim = ' ') {
    vector<string> tokens;
    istringstream iss(input);
    string token;
    while (getline(iss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

Database *createNewDatabaseMenu() {
    string databaseName, *opts, menuHeader, msg;

    newMessage("Insert database name:");
    cout << GRAY << "-> " << RESET;
    cin >> databaseName;
    Database *database = new Database(databaseName);
    //TODO Ask user for database format
    int opt = -1;
    while (opt != 0) {
        menuHeader = "     Add a table to \033[35m'" + databaseName + "'\033[0m?    ";
        opts = new string[]{menuHeader, "[1] Yes", "[0] No"};
        int formula = menuHeader.length() - 8 + menuHeader.length() % 2 - 1; // za pravilnu velicinu tabele
        newMenu(formula, 3, opts);
        cout << GRAY << "-> " << RESET;
        cin >> opt;

        if (opt == 1) {
            newMessage("Insert table name:");
            cout << GRAY << "-> " << RESET;
            string tableName;
            cin >> tableName;

            Table t = Table(tableName);

            msg = "     Insert columns for \033[35m" + tableName + "\033[0m    ";
            newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
            string header;
            cin.ignore();
            cout << GRAY << "-> " << RESET;
            getline(std::cin, header);
            vector<string> unos = splitString(header);
            for (const string &u: unos) {
                t.addHeader(u);
            }


            msg = "     Insert data for \033[35m" + tableName + "\033[0m    ";
            newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
            for (const string &u: unos) {
                cout << u << ' ';
            }
            cout << endl;

            string row;
            while (true) {
                getline(std::cin, row);
                if (row.empty())
                    break;
                Record r = Record();
                vector<string> rowData = splitString(row);
                for (const string &d: rowData) {
                    r.addData(d);
                }
                t.addRecord(r);
            }

            msg = "Table " + tableName + " successfully created.";
            newMessageGreen(msg);

            database->addTable(t);
        }

    }
    delete[] opts;

    string successfulCreation = "Database '" + databaseName + "' successfully created.";
    newMessageGreen(successfulCreation);
    return database;
}

Database *loadDatabase() {
    // TODO: User can load database from existing
    return nullptr;
}

void sqlQuery() {
//    cin.ignore();
    QueryEditor::getInstance().start();
}

void mainMenu() {
    Database *database = nullptr;
    int opt = 1;
    string *opts = new string[]{"MAIN MENU", "[1] Create a new database", "[2] Load database", "[0] Exit"};
    while (!database) {
        newMenu(34, 4, opts, 4);
        cout << GRAY << "-> " << RESET;
        cin >> opt;
        switch (opt) {
            case 1:
                database = createNewDatabaseMenu();
                break;
            case 2:
                database = loadDatabase();
                break;
            case 0:
                newMessageGreen("Program exited successfully.");
                return;
        }
    }
    cout << *database;


    opts = new string[]{"MAIN MENU", "[1] SQL Query", "[2] Export database", "[0] Exit"};
    newMenu(34, 4, opts);
    bool optionSelected = false;
    while (!optionSelected) {
        cin >> opt;
        switch (opt) {
            case 1:
                cin.ignore();
                sqlQuery();
                optionSelected = true;
                break;
            case 2:
                //TODO: After making formats, consider exporting
                optionSelected = true;
                break;
            case 0:
                newMessageGreen("Program exited successfully.");
                return;
        }
    }
    delete[] opts;
    delete database;
}

int main() {
    mainMenu();
    return 0;
}



