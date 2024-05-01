#ifndef PRVIPROJEKAT_TABLE_H
#define PRVIPROJEKAT_TABLE_H

#include <vector>
#include "Record.h"
#include "Errors.h"
#include <algorithm>
#include <regex>
#include <iomanip>
#include <numeric>

class Table {
public: // todo Can't contain numbers in table name
    Table(const string &name) : name(name), records(), header() {}

    Table(const Table &t) {
        copy(t);
    }

    Table(Table &&t) {
        move(t);
    }

    // returns a table of a single column
    Table *getSubTable(const string &colName) const {
        Table *t = new Table(this->getName());

        int columnIndex = -1;
        for (int i = 0; i < this->header.size(); i++) {
            if (regex_match(this->header.at(i), regex(colName, regex_constants::icase))) {
                t->addHeader(this->header.at(i));
                columnIndex = i;
                break;
            }
        }
        if (columnIndex == -1) {
            throw EBadArgumentsException("[ERROR] Column " + colName + " does not exist inside " + this->name);
        }

        for (const auto &record: this->records) {
            Record r = Record();
            r.addData(record.getData().at(columnIndex));
            t->addRecord(r);
        }
        return t;
    }

    static Table *getMergedTable(const Table *t1, const Table *t2) {
        Table *finalTable = new Table(t1->name);
        for (const string &h1: t1->header) {
            finalTable->addHeader(h1);
        }
        for (const string &h2: t2->header) {
            finalTable->addHeader(h2);
        }
        for (int i = 0; i < t1->records.size(); i++) {
            Record record = Record();

            for (const auto &str1: t1->records.at(i).getData()) {
                record.addData(str1);
            }
            for (const auto &str2: t2->records.at(i).getData()) {
                record.addData(str2);
            }
            finalTable->addRecord(record);
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
        t.tablePrint();

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


    void tablePrint() const {
        const char verticalLine = '\xB3';

        vector<int> width = getWidthVectorForTablePrinting();
        if(width.size() == 1){
            int tableNameLen = this->name.length();
            width[0] = max(width[0],tableNameLen);
        }

        upperPartTablePrint(width);

        int headerSize = std::accumulate(width.begin(), width.end(), 0);
        headerSize += width.size() - 1;


        int paddingHeader = headerSize - name.size();
        int leftPaddingHeader = paddingHeader / 2;
        int rightPaddingHeder = paddingHeader - leftPaddingHeader + 1;
        cout << verticalLine << "\033[35m"
             << std::setw(leftPaddingHeader + name.size()) << std::right << name << "\033[0m"
             << std::setw(rightPaddingHeder) << verticalLine << endl;


        middlePartTablePrint(width, true);

        cout << verticalLine;
        for (int i = 0; i < header.size(); i++) {
            int padding = width[i] - header[i].size();
            int leftPadding = padding / 2;
            int rightPadding = padding - leftPadding + 1;
            cout << std::setw(leftPadding + header[i].size()) << std::right << header[i] << std::setw(rightPadding)
                 << verticalLine;
        }
        cout << endl;

        if(records.empty())
            bottomPartTablePrint(width);
        else
            middlePartTablePrint(width);

        for (auto it = records.begin(); it != records.end(); ++it) {
            const auto &record = *it;
            cout << verticalLine;
            for (int i = 0; i < record.getData().size(); i++) {
                int padding = width[i] - record.getData()[i].size();
                int leftPadding = padding / 2;
                int rightPadding = padding - leftPadding + 1;
                cout << std::setw(leftPadding + record.getData()[i].size()) << std::right << record.getData()[i]
                     << std::setw(rightPadding)
                     << verticalLine;
            }
            cout << endl;
            if (it < records.end() - 1)
                middlePartTablePrint(width);
            else
                bottomPartTablePrint(width);
        }

    }

    void upperPartTablePrint(vector<int> width) const {
        const char topLeftCorner = '\xDA';
        const char topRightCorner = '\xBF';
        const char horizontalLine = '\xC4';

        cout << topLeftCorner;
        for (int i = 0; i < width.size() - 1; ++i) {
            for (int j = 0; j < width[i]; ++j) {
                cout << horizontalLine;
            }
            cout << horizontalLine;
        }
        for (int j = 0; j < width[width.size() - 1]; ++j) {
            cout << horizontalLine;
        }
        cout << topRightCorner << endl;
    }

    void middlePartTablePrint(vector<int> width, bool up = false) const {
        const char middleLeftCorner = '\xC3';
        const char middleDown = '\xC2';
        const char middleCenter = '\xC5';
        const char middleRightCorner = '\xB4';
        const char horizontalLine = '\xC4';

        cout << middleLeftCorner;
        for (int i = 0; i < width.size() - 1; ++i) {
            for (int j = 0; j < width[i]; ++j) {
                cout << horizontalLine;
            }
            cout << (up ? middleDown : middleCenter);
        }
        for (int j = 0; j < width[width.size() - 1]; ++j) {
            cout << horizontalLine;
        }
        cout << middleRightCorner << endl;
    }

    void bottomPartTablePrint(vector<int> width) const {
        const char bottomLeftCorner = '\xC0';
        const char bottomRightCorner = '\xD9';
        const char horizontalLine = '\xC4';
        const char middleUp = '\xC1';

        cout << bottomLeftCorner;
        for (int i = 0; i < width.size() - 1; ++i) {
            for (int j = 0; j < width[i]; ++j) {
                cout << horizontalLine;
            }
            cout << middleUp;
        }
        for (int j = 0; j < width[width.size() - 1]; ++j) {
            cout << horizontalLine;
        }
        cout << bottomRightCorner << endl;
    }

    vector<int> getWidthVectorForTablePrinting(int padding = 2) const {
        vector<int> width;
        width.resize(header.size());
        fill(width.begin(), width.end(), 0);

        for (int i = 0; i < width.size(); ++i) {
            width[i] = header[i].size();
        }

        for (const Record &record: records) {
            for (int j = 0; j < record.getData().size(); j++) {
                int strSize = record.getData()[j].length();
                width[j] = max(width[j], strSize);
            }
        }

        for_each(width.begin(), width.end(), [padding](int &value) {
            value += padding;
        });

        return width;
    }

    static void tablePrint1(ostream &os, const Table &t) {
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
    }


};


#endif //PRVIPROJEKAT_TABLE_H
