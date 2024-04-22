#ifndef PRVIPROJEKAT_TABLE_H
#define PRVIPROJEKAT_TABLE_H

#include <vector>
#include "Record.h"

class Table {
public:
    Table(const string &name);

    Table(const Table &t) {
        copy(t);
    }

    Table(Table &&t) {
        move(t);
    }

    Table &operator=(const Table &t) {
        copy(t);
        return *this;
    }

    Table &operator=(Table &&t) {
        move(t);
        return *this;
    }

    void addRecord(const Record &r) {
        records.push_back(r);
    }
    void addHeader(const string& s) {
        header.push_back(s);
    }

    friend ostream &operator<<(ostream &os, const Table &t) {
        for (auto it = t.header.begin(); it != t.header.end(); ++it) {
            if (it != t.header.begin())
                os << ' ';
            os << *it;
        }
        os << endl;
        for (auto it = t.records.begin(); it != t.records.end(); ++it) {
            os << *it;
        }
        os << endl;
        return os;
    }


private:
    string name;
    vector<Record> records;
    vector<string> header;


    void copy(const Table &t) {
        this->records = vector<Record>();
        for (const Record &s: t.records) {
            this->records.push_back(s);
        }

        this->header = vector<string>();
        for (const string &s: t.header) {
            this->header.push_back(s);
        }
    }

    void move(Table &t) {
        this->records = t.records;
        t.records = vector<Record>();
    }
};


#endif //PRVIPROJEKAT_TABLE_H
