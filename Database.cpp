#include "Database.h"

Database::Database(const string &name) : name(name) {}

const string &Database::getName() const {
    return name;
}
