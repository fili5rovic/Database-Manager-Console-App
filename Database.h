#ifndef PRVIPROJEKAT_DATABASE_H
#define PRVIPROJEKAT_DATABASE_H

#include <iostream>
#include "Table.h"
using namespace std;

class Database {
public:
    Database(const string &name);

    const string &getName() const;

private:
    string name;
    std::vector<Table> tables;
};


#endif //PRVIPROJEKAT_DATABASE_H
