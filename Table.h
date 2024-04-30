#ifndef PRVIPROJEKAT_TABLE_H
#define PRVIPROJEKAT_TABLE_H

#include <vector>
#include "Record.h"
#include "Errors.h"
#include <algorithm>

class Table {
public: // Can't contain numbers in table name
    Table(const string &name) : name(name), records(), header() {}

    Table(const Table &t) {
        copy(t);
    }

    Table(Table &&t) {
        move(t);
    }

    Table *getSubTable(const string &colName) const {
        Table *t = new Table(this->getName());
        t->addHeader(colName);
        // todo does not work because the input is lowercase not like column names exactly
        auto colIterator = std::find(this->header.begin(), this->header.end(), colName);
        int columnIndex;
        if (colIterator == header.end()) {
            throw EBadArgumentsException("[ERROR] Column " + colName + " does not exist inside " + this->name);
        }
        columnIndex = std::distance(this->header.begin(), colIterator);
        for (const auto &record: this->records) {
            Record r = Record();
            r.addData(record.getData().at(columnIndex));
            t->addRecord(r);
        }
        return t;
    }

    static Table* getMergedTable(const Table *t1, const Table* t2) {
        Table* finalTable = new Table(t1->name);
        for(const string& h1 : t1->header) {
            finalTable->addHeader(h1);
        }
        for(const string& h2 : t2->header) {
            finalTable->addHeader(h2);
        }
        for(const Record& r1 : t1->records) {
            Record r = Record();

            for(const string& str : r1.getData()) {
                r.addData(str);
            }

            for(const Record& r2 : t2->records) {
                for(const string& str : r2.getData()) {
                    r.addData(str);
                }
            }
            finalTable->addRecord(r);
        }
        return finalTable;

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

    void addHeader(const string &s) {
        header.push_back(s);
    }


    friend ostream &operator<<(ostream &os, const Table &t) {
        os << "Table \033[35m" << t.name << "\033[0m:" << endl;
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


    const string &getName() const {
        return this->name;
    }

    const vector<string> &getTableHeaders() const {
        return this->header;
    }

    const vector<Record> &getTableRecords() const {
        return this->records;
    }


private:
    string name;
    vector<Record> records;
    vector<string> header;


    void copy(const Table &t) {
        this->name = t.name;
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
        this->name = t.name;
        this->records = t.records;
        this->header = t.header;
        t.records = vector<Record>();
        t.name = "";
    }


};


#endif //PRVIPROJEKAT_TABLE_H
