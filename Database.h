#ifndef PRVIPROJEKAT_DATABASE_H
#define PRVIPROJEKAT_DATABASE_H

#include <iostream>
#include <unordered_map>
#include "Table.h"
#include "StringManipulator.h"

using namespace std;

class Database {
public:
    Database(const string &name) : name(name), tables() {}

    Database(const Database &d) = delete;

    Database(Database &&d) = delete;

    Database &operator=(const Database &d) = delete;

    Database &operator=(Database &&d) = delete;

    const string &getName() const;

    void addTable(const Table &t) {
        tables.insert({t.getName(), new Table(t)});
    }

    void removeTable(const string &name) {
        if (tables.find(name) == tables.end())
            return;
        tables.erase(name);
    }

    friend ostream &operator<<(ostream &os, const Database &d) {
        os << endl << "Database \033[35m" << d.name << "\033[0m:" << endl;
        os << "---------------------------------------" << endl;
        for (auto &table: d.tables) {
            os << *table.second << endl;
        }
        os << "---------------------------------------" << endl;
        return os;
    }

    Table* tryGettingTableByNameCaseI(const string &tableName) const {
        for (const auto &pair: tables) {
            if (regex_match(pair.first, regex(tableName, regex_constants::icase)))
                return pair.second;
        }
        return nullptr;
    }

    vector<string> getAllTableNames() const {
        vector<string> retVector;
        retVector.reserve(tables.size());
        for (const auto &pair: tables) {
            retVector.push_back(pair.first);
        }
        return retVector;
    }

    vector<string> getAllHeaders() const {
        vector<string> retVector;
        for (const auto &header: tables) {
            for (const auto &str: header.second->getTableHeaders()) {
                retVector.push_back(str);
            }
        }
        return retVector;
    }

    const unordered_map<string, Table*>& getTablePairs() const {
        return tables;
    }

private:
    const string name;
    unordered_map<string, Table*> tables;
};


#endif //PRVIPROJEKAT_DATABASE_H
