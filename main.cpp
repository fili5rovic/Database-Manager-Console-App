#include <iostream>
#include "Database.h"
#include "QueryEditor.h"
#include "Equal.h"
#include "SQLFormat.h"
#include <vector>

using namespace std;

const string GRAY = "\033[37m";
const string RESET = "\033[0m";

void printActualInput(string selectedMenuOptionString, int substrNum = 4) {
    substrNum = (selectedMenuOptionString.length() > substrNum ? substrNum : 0);
    const string moveUpCursor = "\033[F";
    const string userInputColor = StringManipulator::instance().CYANCOLOR();
    // todo move it 4 places to the rigth don't print arrow
    cout << moveUpCursor << GRAY << "-> " << userInputColor << selectedMenuOptionString.substr(substrNum) << RESET << endl;
}

Database *createNewDatabaseMenu() {
    string databaseName, *opts, menuHeader, msg;

    StringManipulator::instance().newMessage("Insert database name:");
    cout << GRAY << "-> " << RESET;
    cin >> databaseName;
    printActualInput(databaseName,0);

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
            printActualInput(opts[1]);
            StringManipulator::instance().newMessage("Insert table name:");
            cout << GRAY << "-> " << RESET;
            string tableName;
            cin >> tableName;

            printActualInput(tableName, 0);

            Table t = Table(tableName);
            string header;
            vector<string> unos;
            while(true) {
                msg = "     Insert columns for \033[35m" + tableName + "\033[0m    ";
                StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
                cin.ignore();
                cout << GRAY << "-> " << RESET;
                getline(std::cin, header);
                printActualInput(header, 0);
                if(header.empty())
                    continue;
                unos = StringManipulator::splitString(header);
                for (const string &u: unos) {
                    t.addHeader(u);
                }
                break;
            }



            msg = "     Insert data for \033[35m" + tableName + "\033[0m    ";
            StringManipulator::instance().newMessage(msg, msg.length() - 8 + msg.length() % 2 - 1);
            for (const string &u: unos) {
                cout << StringManipulator::instance().BOLDCOLOR() <<  u << ' ';
            }
            cout << endl;

            string row;
            while (true) {
                getline(std::cin, row);
                if (row.empty())
                    break;
                Record r = Record();
                vector<string> rowData = StringManipulator::splitString(row);

                if(rowData.size() != unos.size()) {
                    cout << "\033[F";
                    cout << StringManipulator::instance().REDCOLOR();
                    cout << row << endl;
                    cout << "[ERROR] Given row has " << (rowData.size() > unos.size() ? "more":"less") << " values than columns." << endl;
                    cout << StringManipulator::instance().RESETCOLOR();
                    cout << "Press any key to continue..." << endl;
                    getch();
                    cout << "\033[F\033[F\033[F";
                    cout << StringManipulator::instance().printspace(30) << endl;
                    cout << StringManipulator::instance().printspace(50) << endl;
                    cout << StringManipulator::instance().printspace(30) << endl;
                    cout << "\033[F\033[F\033[F";
                    continue;
                }
                printActualInput(row, 0);

                for (const string &d: rowData) {
                    r.addData(d);
                }
                t.addRecord(r);
            }

            msg = "Table " + tableName + " successfully created.";
            StringManipulator::instance().newMessageGreen(msg);

            database->addTable(t);
        } else {
            printActualInput(opts[1]);
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
                printActualInput(opts[1]);
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
            case 2: {
                SQLFormat exportedDatabase = SQLFormat(database);
                exportedDatabase.exportDatabase();
                break;
                }
            case 0:
                printActualInput(opts[3]);
                StringManipulator::instance().newMessageGreen("Program exited successfully.");
                userWantsToQuit = true;
        }
    }
    delete[] opts;
    delete database;
}

int main() {
    mainMenu(); // todo created should also be colored in some way



//    Table* table = new Table("Student");
//
//    table->addHeader("Index");
//    table->addHeader("Name");
//    table->addHeader("Surname");
//    table->addHeader("Average Grades");
//
//    Record r1 = Record();
//    r1.addData("192");
//    r1.addData("Filip");
//    r1.addData("Petrovic");
//    r1.addData("8.1");
//
//    table->addRecord(r1);
//
//    Record r2 = Record();
//    r2.addData("589");
//    r2.addData("Jovan");
//    r2.addData("Mosurovic");
//    r2.addData("8.0");
//
//    table->addRecord(r2);
//
//    // Retrieve a reference to the second record and modify it
//    Record& rec = table->getRecordByIndex(1);
//    rec.getDataReference().at(0) = "SOMETHING";
//
//    cout << *table;
    return 0;



}