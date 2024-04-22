#include <iostream>
#include <utility>
#include <sstream>
#include "Database.h"

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
const string PURPLE = "\033[35m";
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

vector<string> splitString(const string& input, const char& delim=' ') {
    vector<string> tokens;
    istringstream iss(input);
    string token;
    while (getline(iss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

Database *createNewDatabase() {
    string databaseName, *opts, menuHeader, tableName, msg;

    newMessage("Insert database name:");
    cout << "-> ";
    cin >> databaseName;
    Database *database = new Database(databaseName);
    //TODO Ask user for database format
    int opt = -1;
    while (opt != 0) {
        menuHeader = "     Would you like to add a table to \033[35m'" + databaseName + "'\033[0m?    ";
        opts = new string[]{menuHeader, "[1] Yes", "[0] No"};
        int formula = menuHeader.length() - 8 + menuHeader.length()%2-1; // za pravilnu velicinu tabele
        newMenu(formula, 3, opts);
        cin >> opt;

        if (opt == 1) {
            newMessage("Insert table name:");
            cin >> tableName;
            Table* t = new Table(tableName);

            msg = "Table " + tableName + " successfully created.";
            newMessageGreen(msg);

            cout << "FORMAT: " << endl;
            cout << "Colomn1 Colomn2 Colomn3 ... ColomnN" << endl;
            cout << "Data1 Data2 Data3 ... DataN" << endl;
            cout << "..... ..... ..... ... ....." << endl;

            msg = "Insert header for " + tableName;
            newMessage(msg);
            string header;
            cin.ignore();
            getline(std::cin, header);
            vector<string> unos = splitString(header);
            for(const string& u: unos) {
                t->addHeader(u);
            }
            msg = "Insert data for " + tableName;
            newMessage(msg);
            for(const string& u: unos) {
                cout << u << ' ';
            }
            cout << endl;

            string row;
            while(true) {
                getline(std::cin, row);
                if(row.empty())
                    break;
                cout << "LINIJA:" << row << endl;
                Record r = Record();
                vector<string> rowData = splitString(row);
                for(const string& d: rowData) {
                    r.addData(d);
                }
                t->addRecord(r);
            }
            cout << *t << endl;
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

void mainMenu() {
    Database *database;
    int opt = 1;
    while (opt) {
        newMenu(34, 4, new string[]{"MAIN MENU", "[1] Create a new database", "[2] Load database", "[0] Exit"}, 4);
        cout << "-> ";
        cin >> opt;
        switch (opt) {
            case 1:
                database = createNewDatabase();
                opt = 0;
                break;
            case 2:
                database = loadDatabase();
                opt = 0;
                break;
            case 0:
                newMessageGreen("Program exited successfully.");
                return;
        }
    }
    delete database;
}


int main() {

//    int seb;
//    cin >> seb;
//
//    system("cls");
//    cin >> seb;
    mainMenu();
//    Table t = Table("Testic");
//    t.addHeader("ID");
//    t.addHeader("Ime");
//    t.addHeader("Prezime");
//
//    Record r1 = Record();
//    r1.addData("3");
//    r1.addData("Filip");
//    r1.addData("Petrovic");
//    t.addRecord(r1);
//
//    Record r2 = Record();
//    r2.addData("4");
//    r2.addData("Jovan");
//    r2.addData("Mosurovic");
//    t.addRecord(r2);
//
//    cout << t << endl;
    return 0;
}
