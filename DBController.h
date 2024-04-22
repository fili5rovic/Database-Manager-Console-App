
#ifndef PRVIPROJEKAT_DBCONTROLLER_H
#define PRVIPROJEKAT_DBCONTROLLER_H

#include "Database.h"

class DBController {
public:
    // ovo ce da bude singleton i imace database umesto u mainu da se uzima
private:
    Database* database;
};
#endif //PRVIPROJEKAT_DBCONTROLLER_H
