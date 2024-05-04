#ifndef PRVIPROJEKAT_CUSTOMFORMAT_H
#define PRVIPROJEKAT_CUSTOMFORMAT_H


#include "Format.h"

class CustomFormat : public Format {
public:
    CustomFormat(const Database *database) : Format(database) {}


    static void createDatabaseFromFile(const string& filePath) {

    }

private:
    void printTableNames(stringstream &ss) const override {
        for (const auto &tablePair: database->getTablePairs()) {
            ss << "START_TABLE \"" + tablePair.first + "\" [" << endl;
            const auto &headers = tablePair.second->getTableHeaders();

            int maxLength = calculateMaxColumnNameLength(tablePair.second->getTableHeaders());

            for (size_t i = 0; i < headers.size(); ++i) {
                ss << std::setw(maxLength + 5) << std::right << ("\"" + headers[i] + "\"") << "\tVARCHAR(50) NOT NULL";
                if (i < headers.size() - 1)
                    ss << ',';
                ss << endl;
            }
            ss << "END_TABLE" << endl << endl;
        }
    }

    void printDataInsertion(stringstream &ss) const override {
        for (const auto &tablePair: database->getTablePairs()) {
            for (const Record &record: tablePair.second->getTableRecords()) {
                ss << "I: \"" << tablePair.first << "\" <- {";
                for (int i = 0; i < record.getData().size(); i++) {
                    ss << "\'" << record.getData()[i];
                    if (i < record.getData().size() - 1)
                        ss << "\',";
                    else
                        ss << "\'};" << std::endl;
                }
            }
        }
    }

    string getFileExtension() const override {
        return ".wyl";
    }

    string getDir() const override {
        return "CustomFormatExports/";
    }


};


#endif //PRVIPROJEKAT_CUSTOMFORMAT_H
