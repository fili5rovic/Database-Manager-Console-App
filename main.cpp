#include <iostream>
#include <utility>
#include "Database.h"
#include "QueryEditor.h"
#include <vector>

using namespace std;

const string GRAY = "\033[37m";
const string RESET = "\033[0m";


Database *createNewDatabaseMenu() {
    string databaseName, *opts, menuHeader, msg;

    StringManipulator::instance().newMessage("Insert database name:");
    cout << GRAY << "-> " << RESET;
    cin >> databaseName;
    Database *database = new Database(databaseName);
    //TODO Ask user for database format
    int opt = -1;
    while (opt != 0) {
        menuHeader = "     Add new table to \033[35m'" + databaseName + "'\033[0m?    ";
        opts = new string[]{menuHeader, "[1] Yes", "[0] No"};
        int formula = menuHeader.length() - 8 + menuHeader.length() % 2 - 1; // za pravilnu velicinu tabele
        StringManipulator::instance().newMenu(formula, 3, opts);
        cout << GRAY << "-> " << RESET;
        cin >> opt;

        if (opt == 1) {
            StringManipulator::instance().newMessage("Insert table name:");
            cout << GRAY << "-> " << RESET;
            string tableName;
            cin >> tableName;

            Table t = Table(tableName);

            msg = "     Insert columns for \033[35m" + tableName + "\033[0m    ";
            StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
            string header;
            cin.ignore();
            cout << GRAY << "-> " << RESET;
            getline(std::cin, header);
            vector<string> unos = StringManipulator::splitString(header);
            for (const string &u: unos) {
                t.addHeader(u);
            }


            msg = "     Insert data for \033[35m" + tableName + "\033[0m    ";
            StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
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
                vector<string> rowData = StringManipulator::splitString(row);
                for (const string &d: rowData) {
                    r.addData(d);
                }
                t.addRecord(r);
            }

            msg = "Table " + tableName + " successfully created.";
            StringManipulator::instance().newMessageGreen(msg);

            database->addTable(t);
        }

    }
    delete[] opts;

    string successfulCreation = "Database '" + databaseName + "' successfully created.";
    StringManipulator::instance().newMessageGreen(successfulCreation);
    return database;
}

Database *loadDatabase() {
    // TODO: User can load database from existing
    return nullptr;
}

void sqlQuery(Database* d) {
//    cin.ignore();
    QueryEditor editor = QueryEditor(d);
    editor.start();
}

void mainMenu() {
    Database *database = nullptr;
    int opt = 1;
    string *opts = new string[]{"MAIN MENU", "[1] Create a new database", "[2] Load database", "[0] Exit"};
    while (!database) {
        StringManipulator::instance().newMenu(34, 4, opts, 4);
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
                StringManipulator::instance().newMessageGreen("Program exited successfully.");
                return;
        }
    }
    cout << *database;


    opts = new string[]{"MAIN MENU", "[1] SQL Query", "[2] Export database", "[0] Exit"};
    bool userWantsToQuit = false;
    while (!userWantsToQuit) {
        StringManipulator::instance().newMenu(34, 4, opts);
        cout << GRAY << "-> " << RESET;
        cin >> opt;
        switch (opt) {
            case 1:
                cin.ignore();
                sqlQuery(database);
                break;
            case 2:
                //TODO: After making formats, consider exporting
                break;
            case 0:
                StringManipulator::instance().newMessageGreen("Program exited successfully.");
                userWantsToQuit = true;
        }
    }
    delete[] opts;
    delete database;
}

int main() {
    mainMenu();
    return 0;



}



