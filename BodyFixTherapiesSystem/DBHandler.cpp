#include "DBHandler.h"
#include "Herb.h"

QSqlDatabase DBHandler::GetBFTDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/BFTDB.db");
    return db;
}