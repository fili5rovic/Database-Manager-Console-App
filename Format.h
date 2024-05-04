#ifndef PRVIPROJEKAT_FORMAT_H
#define PRVIPROJEKAT_FORMAT_H

#include <fstream>
#include <filesystem>
#include "Database.h"

class Format {
public:
    Format(const Database* database) : database(database) {}

    void message() const {
        cout << "Successfully exported " << database->getName() << getFileExtension() << endl;
    }

    void exportDatabase() const {
        filesystem::create_directories(getDir()); // This will create the directory if it does not exist
        ofstream outFile(getDir() + database->getName()+getFileExtension()); // .wyl
        if(!outFile.is_open())
            throw EFileNotOpen("[RUNTIME_ERROR] Can't open file.");
        std::stringstream ss;

        printDatabase(ss);

        outFile << ss.str();
        outFile.close();
        message();
    }





protected:
    const Database* database;

    int calculateMaxColumnNameLength(const vector<string> headers) const {
        int maxLength = 0;
        for (const auto &header : headers) {
            if (header.length() > maxLength) {
                maxLength = header.length();
            }
        }
        return maxLength;
    }

    virtual void printDatabase(stringstream& ss) const {
        printTableNames(ss);
        printDataInsertion(ss);
    }



    virtual void printTableNames(stringstream& ss) const = 0;
    virtual void printDataInsertion(stringstream& ss) const = 0;
    virtual string getFileExtension() const = 0;
    virtual string getDir() const = 0;




};


#endif //PRVIPROJEKAT_FORMAT_H
