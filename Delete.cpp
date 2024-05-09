//
// Created by fili5 on 19.4.2024..
//

#include "Delete.h"

Delete::Delete(const string &input, shared_ptr<Database> database) : Statement(input, database) {}

void Delete::executingQuery(const smatch &matches) const {
    string tableName = matches[1];
    string whereStr = matches[2];

    Filter f(this->table, whereStr);
    shared_ptr<Table> filterTable = f.getFilteredTable();


    vector<int> indices;
    indices.reserve(filterTable->getTableRecords().size());
    for (const Record &rec: filterTable->getTableRecords()) {
        indices.push_back(table->getRecordIndex(rec.getData()));
    }
    if(indices.empty()) {
        cout << *table << endl;
        cout << "No rows affected." << endl;
        return;
    }

    std::sort(indices.begin(), indices.end());
    std::reverse(indices.begin(), indices.end());

    string headerStr = "This will delete " + to_string(indices.size()) + (indices.size() == 1 ? " row":" rows") + ". Proceed?";
    vector<string> opts = {"Security Warning", headerStr, "[1] Yes", "[0] Go back"};
    StringManipulator::instance().newMenu(44, opts);
    string opt;
    cin >> opt;
    if(opt == "1") {
        cout << "\033[F" << "Yes" << endl;
        for(int index: indices) {
            table->removeRecordFromIndex(index);
        }
        cout << *table << endl;

        string msg;
        if(indices.size() == 1)
            msg = "1 row deleted.";
        else
            msg = to_string(indices.size()) + " rows deleted.";
        StringManipulator::instance().newMessage(msg);
    }
}
