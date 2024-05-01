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
    Database(const Database& d) = delete;
    Database(Database&& d) = delete;
    Database& operator=(const Database& d) = delete;
    Database& operator=(Database&& d) = delete;

    const string &getName() const;

    void addTable(const Table& t) {
        tables.insert({t.getName(),t});
    }
    void removeTable(const string& name) {
        if(tables.find(name) == tables.end())
            return;
        tables.erase(name);
    }

    friend ostream& operator<<(ostream& os, const Database& d) {
        os << "Database \033[35m" << d.name << "\033[0m:" << endl;
        os << "-----------------------" << endl;
        for(auto& table : d.tables) {
            os << table.second << endl;
        }
        os << "-----------------------" << endl;
        return os;
    }

    Table* tryGettingTableByNameCaseI(const string& tableName){
        for(const auto& pair : tables) {
            if(StringManipulator::instance().caseInsensitiveStringCompare(pair.first, tableName)) // todo regex here
                return new Table(pair.second);
        }
        return nullptr;
    }

    vector<string> getAllTableNames() const {
        vector<string> retVector;
        retVector.reserve(tables.size());
        for(const auto& pair : tables) {
            retVector.push_back(pair.first);
        }
        return retVector;
    }

    vector<string> getAllHeaders() const {
        vector<string> retVector;
        for(const auto& header: tables) {
            for(const auto& str: header.second.getTableHeaders()) {
                retVector.push_back(str);
            }
        }
        return retVector;
    }

private:
    string name;
    unordered_map<string,Table> tables;
};


#endif //PRVIPROJEKAT_DATABASE_H
