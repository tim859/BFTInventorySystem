#include "HerbDBHandler.h"
#include "Herb.h"

QSqlQuery HerbDBHandler::GetAllHerbsFromDB() {

    QSqlDatabase db = GetBFTDB();
    QSqlQuery herbQuery;

    if (!db.open()) {
        return herbQuery;
    }

    herbQuery.exec("SELECT * FROM herbs");

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
    insertQuery.prepare("INSERT INTO herbs (herb_name, category, current_stock_total, cost_per_gram, show_supplier_orders, starting_balance, total_grams_purchased, total_grams_sold, preferred_supplier, starting_cost)"
                        "VALUES (:herb_name, :category, :current_stock_total, :cost_per_gram, :show_supplier_orders, :starting_balance, :total_grams_purchased, :total_grams_sold, :preferred_supplier, :starting_cost)");

    // bind values to placeholders
    insertQuery.bindValue(0, QString::fromStdString(newHerb.name));
    insertQuery.bindValue(1, QString::fromStdString(newHerb.category));
    insertQuery.bindValue(2, newHerb.currentStockTotal);
    insertQuery.bindValue(3, newHerb.costPerGram.GetMills());
    insertQuery.bindValue(4, newHerb.showSupplierOrders);
    insertQuery.bindValue(5, newHerb.startingBalance);
    insertQuery.bindValue(6, newHerb.totalGramsPurchased);
    insertQuery.bindValue(7, newHerb.totalGramsSold);
    insertQuery.bindValue(8, QString::fromStdString(newHerb.preferredSupplier));
    insertQuery.bindValue(9, newHerb.startingCost.GetMills());

    // execute the query and check for failure
    if (insertQuery.exec()) {
        return true;
    }

    return false;
}

bool HerbDBHandler::DeleteHerbFromDB()
{
    return false;
}

bool HerbDBHandler::EditHerbInDB()
{
    return false;
}
