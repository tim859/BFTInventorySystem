#include "DBHandler.h"
#include "Herb.h"
#include "Formula.h"
#include <iostream>
#include <qsqlerror.h>

DBHandler* DBHandler::instance = nullptr;

// ---------- general functions ----------

DBHandler::DBHandler(std::string connName)
{
    db = QSqlDatabase::addDatabase("QSQLITE", QString::fromStdString(connName));
    db.setDatabaseName("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/BFTDB.db");

    if (!db.open()) {
        std::cout << "failed to open database with connection name: " << connName << "\n";
    }
}

DBHandler::~DBHandler()
{
    db.close();
}

DBHandler& DBHandler::GetInstance() {
    if (!instance) {
        instance = new DBHandler("DBHandlerConn");
    }

    return *instance;
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
    insertQuery.bindValue(0, QString::fromStdString(newHerb.name));
    insertQuery.bindValue(1, QString::fromStdString(newHerb.category));
    insertQuery.bindValue(2, newHerb.currentStockTotal);
    insertQuery.bindValue(3, newHerb.costPerGram.GetMills());
    insertQuery.bindValue(4, QString::fromStdString(newHerb.preferredSupplier));

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
    updateQuery.bindValue(":cost_per_gram", editedHerb.costPerGram.GetMills());
    updateQuery.bindValue(":preferred_supplier", QString::fromStdString(editedHerb.preferredSupplier));
    updateQuery.bindValue(":rowid", editedHerb.id);

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

int DBHandler::GetRowsInHerbTable()
{
    QSqlQuery herbQuery(db);
    herbQuery.prepare("SELECT COUNT(*) FROM herbs");

    if (herbQuery.exec()) {
        int rowCount = herbQuery.value(0).toInt();
        return rowCount;
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
    insertQuery.prepare("INSERT INTO formulas (patient_name, cost_of_herbs, cost_to_patient, list_of_herbs)"
        "VALUES (:patient_name, :cost_of_herbs, :cost_to_patient, :list_of_herbs)");

    insertQuery.bindValue(":patient_name", QString::fromStdString(newFormula.patientName));
    insertQuery.bindValue(":cost_of_herbs", newFormula.costOfHerbs.GetMills());
    insertQuery.bindValue(":cost_to_patient", newFormula.costToPatient.GetMills());

    // create string from list of herbs and list of amounts
    std::string herbAndAmountString = "";
    for (int i = 0; i < newFormula.listOfHerbs->size(); i++) {
        // get rowID of the ith herb in the list and convert it to a std::string
        std::string herbRowID = std::to_string(static_cast<long long>((*newFormula.listOfHerbs)[i].id));
        // get amount of the ith herb in the list and convert it to a std::string
        std::string herbAmount = std::to_string(static_cast<long long>(newFormula.listOfHerbAmounts[i]));
        
        herbAndAmountString += herbRowID + "-" + herbAmount + ",";
    }

    // replace last character (last character should be a comma at this point) of herbListString with e to denote end of string
    herbAndAmountString[herbAndAmountString.size() - 1] = 'e';

    insertQuery.bindValue(":list_of_herbs", QString::fromStdString(herbAndAmountString));

    if (insertQuery.exec()) {
        return true;
    }

    qDebug() << "Query execution failed:" << insertQuery.lastError().text();
    return false;
}

int DBHandler::GetRowsInFormulaTable()
{
    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT COUNT(*) FROM formulas");

    if (selectQuery.exec()) {
        int rowCount = selectQuery.value(0).toInt();
        return rowCount;
    }

    return 0;
}