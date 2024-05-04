#ifndef PRVIPROJEKAT_SQLFORMAT_H
#define PRVIPROJEKAT_SQLFORMAT_H

#include <fstream>
#include "Format.h"

class SQLFormat : Format {
public:
    SQLFormat(const Database* database) : Format(database) {}

private:
    void printTableNames(std::stringstream& ss) const {
        for(const auto& tablePair : database->getTablePairs()) {
            ss << "CREATE TABLE IF NOT EXISTS \"" + tablePair.first + "\" (" << std::endl;
            const auto& headers = tablePair.second->getTableHeaders();
            for(size_t i = 0; i < headers.size(); ++i) {
                ss << "\"" << headers[i] << "\"\tVARCHAR(50) NOT NULL";
                if(i < headers.size() - 1)
                    ss << ',';
                ss << std::endl;
            }
            ss << ");" << std::endl;
        }
    }

    void printDataInsertion(stringstream& ss) const {
        for(const auto& tablePair : database->getTablePairs()) {
            for(const Record& record : tablePair.second->getTableRecords()) {
                ss << "INSERT INTO \"" << tablePair.first << "\" VALUES (";
                for(int i = 0; i < record.getData().size(); i++) {
                    ss << "\'" << record.getData()[i];
                    if(i < record.getData().size() - 1)
                        ss << "\',";
                    else
                        ss << "\');" << std::endl;
                }
            }
        }
    }
};


#endif //PRVIPROJEKAT_SQLFORMAT_H
