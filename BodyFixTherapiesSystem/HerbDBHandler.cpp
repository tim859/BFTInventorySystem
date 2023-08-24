#include "HerbDBHandler.h"
#include "Herb.h"
#include <iostream>
#include <qsqlerror.h>

HerbDBHandler::HerbDBHandler()
{
    db = GetBFTDB();
}

QSqlQuery HerbDBHandler::GetAllHerbsFromDB() {

    QSqlQuery herbQuery;

    if (!db.open()) {
        return herbQuery;
    }

    herbQuery.exec("SELECT rowid, * FROM herbs");

    return herbQuery;
    db.close();
}

QSqlQuery HerbDBHandler::SearchDBForHerbs()
{
    return QSqlQuery();
}

bool HerbDBHandler::AddHerbToDB(Herb newHerb)
{
    QSqlQuery insertQuery;
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

bool HerbDBHandler::EditHerbInDB(Herb editedHerb)
{
    QSqlQuery updateQuery;
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

bool HerbDBHandler::DeleteHerbFromDB(int rowID)
{
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM herbs WHERE rowid = :rowid");
    deleteQuery.bindValue(":rowid", rowID);

    if (deleteQuery.exec()) {
        return true;
    }

    return false;
}

int HerbDBHandler::GetRowsInHerbTable()
{
    QSqlQuery herbQuery;
    herbQuery.prepare("SELECT COUNT(*) FROM herbs");

    if (herbQuery.exec()) {
        int rowCount = herbQuery.value(0).toInt();
        return rowCount;
    }

    return 0;
}
