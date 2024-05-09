#ifndef PRVIPROJEKAT_FORMAT_H
#define PRVIPROJEKAT_FORMAT_H

#include <fstream>
#include <filesystem>
#include "Database.h"

class Format {
public:
    Format(const shared_ptr<Database> database) : database(database) {}

    void message(const string& fileName, const string& path) const {
        stringstream ss;
        ss << "Successfully exported: " << path << fileName;
        StringManipulator::instance().newMessageGreen(ss.str(),ss.str().length() + ss.str().length()%2 + 4);
    }

    void exportDatabase() const {
        string path = askUserForPath();
        string fileName = askUserForFileName();
        filesystem::create_directories(path);
        ofstream outFile(path + fileName); // .wyl
        if(!outFile.is_open())
            throw EFileNotOpen("[RUNTIME_ERROR] Can't open file.");
        std::stringstream ss;

        printDatabase(ss);

        outFile << ss.str();
        message(fileName,path);
        outFile.close();
    }

    string askUserForPath() const {
        string path;
        while(true) {
            StringManipulator::instance().newMenu(60, {"Enter relative path to exporting directory:", "Type 'default' to export to the default directory.","Example: ./exports/"});
            cout << "./";
            cin.ignore();
            getline(cin, path);
            if(regex_match(path, regex("^default$", regex_constants::icase))) {
                path = getDir();
            } else {
                if(!regex_match(path, regex("^[a-zA-Z0-9\\/_-]+$"))) {
                    StringManipulator::instance().newMessage("Invalid path syntax. Example: ./exports/");
                    continue;
                }
                path = "./" + path;
                if(!filesystem::exists(path)) {
                    filesystem::create_directories(path);
                }
            }
            break;
        }
        return path;
    }

    string askUserForFileName() const {
        string fileName;
        while(true) {
            StringManipulator::instance().newMenu(44, {"Enter file name:", "Type 'default' to use database name."});
            cout << "->";
            getline(cin, fileName);
            if(regex_match(fileName, regex("^default$", regex_constants::icase))) {
                fileName = database->getName();
            } else if(!regex_match(fileName, regex("^[a-zA-Z0-9_-]+$"))){
                StringManipulator::instance().newMessage("Invalid file name syntax. Example: myDatabase");
                continue;
            }
            fileName += getFileExtension();
            break;
        }
        return fileName;
    }



protected:
    const shared_ptr<Database> database;

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
