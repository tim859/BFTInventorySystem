#include "DBHandler.h"
#include "Herb.h"
#include "Formula.h"
#include <iostream>
#include <QtSql/qsqlerror.h>
#include <QCoreApplication>
#include <QDir>

DBHandler* DBHandler::instance = nullptr;
std::string DBHandler::connName = "DBHandlerConn";

// ---------- general functions ----------

DBHandler::DBHandler() :
    // for debug
    settings("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/config.ini", QSettings::IniFormat)

    // for release
    // settings("./config.ini", QSettings::IniFormat)
{
    UpdateDatabase();
}

DBHandler::~DBHandler()
{
    db.close();
}

DBHandler& DBHandler::GetInstance() {
    if (!instance) {
        instance = new DBHandler();
    }

    return *instance;
}

void DBHandler::UpdateDatabase()
{
    // check if there is already an open database
    if (QSqlDatabase::contains(QString::fromStdString(connName))) {
        // close and remove existing database
        QSqlDatabase::database(QString::fromStdString(connName)).close();
        QSqlDatabase::removeDatabase(QString::fromStdString(connName));
    }

    // add new database connection
    db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(connName));

    // set database path
    std::string dbPath = settings.value("SETTINGS/DatabasePath", "./BFTDB.db").toString().toStdString();
    db.setDatabaseName(QString::fromStdString(dbPath));


    if (!db.open()) {
        std::cout << "failed to open database with connection name: " << connName << "\n";
    }
}

std::string DBHandler::GetHerbsAndAmountsAsDBString(std::vector<Herb>* listOfHerbs, std::vector<int> listOfHerbAmounts)
{
    // check for empty list
    if (listOfHerbs->empty() || listOfHerbAmounts.empty()) {
        return "e";
    }

    // create string from list of herbs and list of amounts
    std::string herbAndAmountString = "";
    for (int i = 0; i < listOfHerbs->size(); i++) {
        // get rowID of the ith herb in the list and convert it to a std::string
        std::string herbRowID = std::to_string(static_cast<long long>((*listOfHerbs)[i].rowID));
        // get amount of the ith herb in the list and convert it to a std::string
        std::string herbAmount = std::to_string(static_cast<long long>(listOfHerbAmounts[i]));

        // concatenate herb, amount and appropriate separators together and append the result to the overall string
        herbAndAmountString += herbRowID + "-" + herbAmount + ",";
    }

    // replace last character (last character should be a comma at this point so needs to be replaced/got rid of anyway) of herbListString with e to denote end of string
    herbAndAmountString[herbAndAmountString.size() - 1] = 'e';

    return herbAndAmountString;
}

// ---------- herb functions ----------

QSqlQuery DBHandler::GetAllHerbsFromDB() 
{
    QSqlQuery selectQuery(db);

    selectQuery.exec("SELECT rowid, * FROM herbs");

    return selectQuery;
}

bool DBHandler::AddHerbToDB(Herb newHerb) 
{
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO herbs (name, category, current_stock_total, cost_per_gram, preferred_supplier)"
        "VALUES (:name, :category, :current_stock_total, :cost_per_gram, :preferred_supplier)");

    // bind values to placeholders
    insertQuery.bindValue(":name", QString::fromStdString(newHerb.name));
    insertQuery.bindValue(":category", QString::fromStdString(newHerb.category));
    insertQuery.bindValue(":current_stock_total", newHerb.currentStockTotal);
    insertQuery.bindValue(":cost_per_gram", newHerb.costPerGram.ToDouble());
    insertQuery.bindValue(":preferred_supplier", QString::fromStdString(newHerb.preferredSupplier));

    // execute the query and check for failure
    if (insertQuery.exec()) {
        return true;
    }

    return false;
}

bool DBHandler::EditHerbInDB(Herb editedHerb) 
{
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE herbs "
        "SET name = :name, "
        "    category = :category, "
        "    current_stock_total = :current_stock_total, "
        "    cost_per_gram = :cost_per_gram, "
        "    preferred_supplier = :preferred_supplier "
        "WHERE rowid = :rowid");

    updateQuery.bindValue(":name", QString::fromStdString(editedHerb.name));
    updateQuery.bindValue(":category", QString::fromStdString(editedHerb.category));
    updateQuery.bindValue(":current_stock_total", editedHerb.currentStockTotal);
    updateQuery.bindValue(":cost_per_gram", editedHerb.costPerGram.ToDouble());
    updateQuery.bindValue(":preferred_supplier", QString::fromStdString(editedHerb.preferredSupplier));
    updateQuery.bindValue(":rowid", editedHerb.rowID);

    if (updateQuery.exec()) {
        return true;
    }

    //std::cout << "SQL error: " << updateQuery.lastError().text().toStdString();
    return false;
}

bool DBHandler::DeleteHerbFromDB(int rowID)
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM herbs WHERE rowid = :rowid");
    deleteQuery.bindValue(":rowid", rowID);

    if (deleteQuery.exec()) {
        return true;
    }

    return false;
}

bool DBHandler::DeleteAllHerbsFromDB()
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM herbs");

    if (deleteQuery.exec()) {
        return true;
    }

    return false;
}

int DBHandler::GetRowIDOfLastRecordInHerbTable()
{
    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT ROWID FROM herbs ORDER BY ROWID DESC LIMIT 1");

    if (selectQuery.exec() && selectQuery.next()) {
        int rowID = selectQuery.value(0).toInt();
        return rowID;
    }

    return 0;
}


// ---------- formula functions ----------

QSqlQuery DBHandler::GetAllFormulasFromDB()
{
    QSqlQuery selectQuery(db);

    selectQuery.exec("SELECT rowid, * FROM formulas");

    return selectQuery;
}

QSqlQuery DBHandler::GetHerbFromDB(int rowID)
{
    QSqlQuery selectQuery(db);

    selectQuery.prepare("SELECT * FROM herbs WHERE rowid = :row_id");
    selectQuery.bindValue(":row_id", rowID);

    if (!selectQuery.exec()) {
        // Handle error if the query execution fails
        std::cout << "Error executing SELECT query:" << selectQuery.lastError().text().toStdString();
    }

    if (selectQuery.next()) {
        return selectQuery;  // Return the QSqlQuery with fetched data
    }
    else {
        std::cout << "No results found for row ID: " << rowID;
        return QSqlQuery();  // Return an empty QSqlQuery object
    }

    return selectQuery;
}

bool DBHandler::AddFormulaToDB(Formula newFormula)
{
    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO formulas (patient_name, list_of_herbs)"
        "VALUES (:patient_name, :list_of_herbs)");

    insertQuery.bindValue(":patient_name", QString::fromStdString(newFormula.patientName));
    insertQuery.bindValue(":list_of_herbs", QString::fromStdString(GetHerbsAndAmountsAsDBString(newFormula.listOfHerbs, newFormula.listOfHerbAmounts)));

    if (insertQuery.exec()) {
        return true;
    }

    qDebug() << "Query execution failed:" << insertQuery.lastError().text();
    return false;
}

bool DBHandler::EditFormulaInDB(Formula editedFormula)
{
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE formulas "
        "SET patient_name = :patient_name, "
        "   list_of_herbs = :list_of_herbs "
        "WHERE rowid = :rowid");

    updateQuery.bindValue(":patient_name", QString::fromStdString(editedFormula.patientName));
    updateQuery.bindValue(":list_of_herbs", QString::fromStdString(GetHerbsAndAmountsAsDBString(editedFormula.listOfHerbs, editedFormula.listOfHerbAmounts)));
    updateQuery.bindValue(":rowid", editedFormula.rowID);

    if (updateQuery.exec()) {
        return true;
    }

    std::cout << "SQL error: " << updateQuery.lastError().text().toStdString();
    return false;
}

bool DBHandler::DeleteFormulaFromDB(int rowID)
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM formulas WHERE rowid = :rowid");
    deleteQuery.bindValue(":rowid", rowID);

    if (deleteQuery.exec()) {
        return true;
    }

    return false;
}

bool DBHandler::DeleteAllFormulasFromDB()
{
    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("DELETE FROM formulas");

    if (deleteQuery.exec()) {
        return true;
    }

    return false;
}

int DBHandler::GetRowIDOfLastRecordInFormulaTable()
{
    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT ROWID FROM formulas ORDER BY ROWID DESC LIMIT 1");

    if (selectQuery.exec() && selectQuery.next()) {
        int rowID = selectQuery.value(0).toInt();
        return rowID;
    }

    return 0;
}