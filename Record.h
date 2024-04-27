#ifndef PRVIPROJEKAT_RECORD_H
#define PRVIPROJEKAT_RECORD_H

#include <iostream>
#include <vector>

using namespace std;

class Record {
public:
    Record() : columns() {}

    Record(const Record &r) {
        copy(r);
    }

    Record(Record &&r) {
        move(r);
    }

    Record &operator=(const Record &r) {
        copy(r);
        return *this;
    }

    Record &operator=(Record &&r) {
        move(r);
        return *this;
    }

    friend ostream &operator<<(ostream &os, const Record &r) {
        for (auto it = r.columns.begin(); it != r.columns.end(); ++it) {
            if (it != r.columns.begin()) {
                os << ' '; // Add space between columns, but not before the first column
            }
            os << *it; // Output the current column
        }
        os << endl;
        return os;
    }

    void addData(const string &col) {
        this->columns.push_back(col);
    }

    vector<string> getData() const {
        return this->columns;
    }

private:
    vector<string> columns;

    void copy(const Record &r) {
        this->columns = vector<string>();
        for (const string &s: r.columns) {
            this->columns.push_back(s);
        }
    }

    void move(Record &r) {
        this->columns = r.columns;
        r.columns = vector<string>();
    }


};


#endif //PRVIPROJEKAT_RECORD_H
