#ifndef PRVIPROJEKAT_DATABASE_H
#define PRVIPROJEKAT_DATABASE_H

#include <iostream>
#include "Table.h"
using namespace std;

class Database {
public:
    Database(const string &name) : name(name), tables() {}
    Database(const Database& d) = delete;
    Database(Database&& d) = delete;
    Database& operator=(const Database& d) = delete;
    Database& operator=(Database&& d) = delete;

    const string &getName() const;

    void addTable(const Table& t) {
        tables.push_back(t);
    }
    friend ostream& operator<<(ostream& os, const Database& d) {
        os << "Database " << d.name << ":" << endl;
        for(const auto & table : d.tables) {
            os << table << endl;
        }
        os << endl;
        return os;
    }

private:
    string name;
    std::vector<Table> tables;
};


#endif //PRVIPROJEKAT_DATABASE_H
