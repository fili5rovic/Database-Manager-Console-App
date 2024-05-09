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

void Delete::runtimeErrors(const string &input) const {
    if(regex_search(input,regex("^\\s*delete\\s+from\\s*(\\w+)\\s*",regex_constants::icase)))
        throw EBadArgumentsException("[RUNTIME_ERROR] Table names can't contain numbers."); // may never enter idk
    throw EBadArgumentsException("[RUNTIME_ERROR] Not yet implemented.");
}

void Delete::checkForSyntaxErrors(const string &query) const {
    // from not detected
    if (!regex_match(query, regex(".*\\s+from\\s*.*", regex_constants::icase))) {
        throw EMissingKeywordsException("[SYNTAX_ERROR] No FROM keyword specified.");
    } // table name not detected
    else if (!regex_search(query, regex(".*\\s+from\\s+(\\w+)\\s*", regex_constants::icase))) {
        throw EMissingArgumentsException("[SYNTAX_ERROR] FROM has no arguments.");
    }
    // multiple keywords
    if (regex_match(query, regex(".*\\bcreate\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] CREATE with DELETE not allowed.");
    } else if (regex_match(query, regex(".*\\binsert\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INSERT with DELETE not allowed.");
    } else if (regex_match(query, regex(".*\\binto\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] INTO with DELETE not allowed.");
    } else if (regex_match(query, regex(".*\\bselect\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] select with DELETE not allowed.");
    } else if (regex_match(query, regex(".*\\btable\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] TABLE with DELETE not allowed.");
    } else if (regex_match(query, regex(".*\\bdelete\\b.*delete.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple DELETE keywords not allowed.");
    } else if (regex_match(query, regex(".*\\bfrom\\b.*\\bfrom\\b.*", regex_constants::icase))) {
        throw EMultipleKeywordsException("[SYNTAX_ERROR] Multiple FROM keywords not allowed.");
    }
}

regex Delete::getRegexPattern() const {
    return regex("^\\s*delete\\s+from\\s*([a-zA-Z_]+)\\s*(?:where\\s+((\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+)(?:\\s+(and|or)\\s*(\\w+)\\s*(\\=|\\<\\>|\\!\\=)\\s*('\\w+'|\"\\w+\"|\\w+))*))?",regex_constants::icase);
}

regex Delete::getRegexForFindingTable() const {
    return regex("^\\s*delete\\s+from\\s*(\\w+)\\s*",regex_constants::icase);
}
