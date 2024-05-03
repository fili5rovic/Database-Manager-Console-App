#ifndef PRVIPROJEKAT_UPDATE_H
#define PRVIPROJEKAT_UPDATE_H


#include "Statement.h"

class Update : public Statement {
public:
    Update(const string &input,Database* database) : Statement(input, database){}

private:

    void executingQuery(const std::smatch &matches) const override {
        string tableName = matches[1];
        string setArgStr = matches[2];
        string whereArgsStr = matches[3];

        vector<string> setArgs = StringManipulator::instance().splitString(setArgStr, ',');
        vector<string> whereArgs = StringManipulator::instance().splitString(whereArgsStr, ',');
        // trim them
    }

    //<editor-fold desc="Error Handling">
    void runtimeErrors(const std::string &input) const override {
        throw EBadArgumentsException("[ERROR] Bad table probably lol");
    }

    void checkForSyntaxErrors(const std::string &query) const override {

    }
    //</editor-fold>

    //<editor-fold desc="Getters">
    regex getRegexForFindingTable() const override {
        return regex("^\\s*update\\s+(\\w+)",regex_constants::icase);
    }


    regex getRegexPattern() const override {
        return regex(
                "^\\s*update\\s+([a-zA-Z_]*)\\s+set\\s+(\\w+\\s*\\=\\s*(?:\\\"\\w+\\\"|\\'\\w+\\')\\s*(?:\\,\\s*\\w+\\s*\\=\\s*(?:\\\"\\w+\\\"|\\'\\w+\\')\\s*)*)(?:where\\s+((\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*(\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?",
                regex_constants::icase);
    }
    //</editor-fold>
};


#endif //PRVIPROJEKAT_UPDATE_H
