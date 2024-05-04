#ifndef PRVIPROJEKAT_FORMAT_H
#define PRVIPROJEKAT_FORMAT_H

#include <fstream>
#include "Database.h"

class Format {
public:
    Format(const Database* database) : database(database) {}

    void exportDatabase() const {
        std::ofstream outFile(database->getName()+".txt");
        if(!outFile.is_open())
            throw EFileNotOpen("[RUNTIME_ERROR] Can't open file.");
        std::stringstream ss;

        printDatabase(ss);

        outFile << ss.str();
        outFile.close();
    }

    virtual void printDatabase(stringstream& ss) const {
        printTableNames(ss);
        printDataInsertion(ss);
    }

    virtual void printTableNames(stringstream& ss) const = 0;
    virtual void printDataInsertion(stringstream& ss) const = 0;


protected:
    const Database* database;
};


#endif //PRVIPROJEKAT_FORMAT_H
