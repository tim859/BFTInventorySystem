#include "HerbHandler.h"
#include "Herb.h"

HerbHandler::HerbHandler()
{
    GetAllHerbs();
}

std::vector<Herb>* HerbHandler::GetAllHerbs() {

    herbList.clear();
    bool tempShowSupplierOrders;
	QSqlQuery herbQuery = herbDBHandler.GetAllHerbsFromDB();

    while (herbQuery.next()) {
        if (herbQuery.value(4) == 0) {
            tempShowSupplierOrders = false;
        }
        else {
            tempShowSupplierOrders = true;
        }
        herbList.emplace_back(Herb(herbQuery.value(0).toString().toStdString(), 
            herbQuery.value(1).toString().toStdString(), 
            herbQuery.value(2).toInt(), 
            Money(herbQuery.value(3).toInt(), 1),
            tempShowSupplierOrders, 
            herbQuery.value(5).toInt(), 
            herbQuery.value(6).toInt(), 
            herbQuery.value(7).toInt(), 
            herbQuery.value(8).toString().toStdString(), 
            Money(herbQuery.value(9).toInt(), 1)));
    }

    return &herbList;
}

std::vector<Herb> HerbHandler::SearchForHerbs()
{
    return std::vector<Herb>();
}

std::vector<Herb>* HerbHandler::GetFilteredHerbs(int index, int filterType)
{
    if (filterType == 0) {
        switch (index) {
        case 1:
            // use lambda functions to define sorting behaviour for each desired sorting specifier
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name < herb2.name;
                });
            break;

        case 2:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category < herb2.category;
                });
            break;

        case 3:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal < herb2.currentStockTotal;
                });
            break;

        case 4:
            // doesnt work at all, rewrite
            std::sort(herbList.begin(), herbList.end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() < herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.startingBalance < herb2.startingBalance;
                });
            break;

        case 6:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.totalGramsPurchased < herb2.totalGramsPurchased;
                });
            break;

        case 7:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.totalGramsSold < herb2.totalGramsSold;
                });
            break;

        case 8:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier < herb2.preferredSupplier;
                });
            break;

        case 9:
            // same problem as above
            std::sort(herbList.begin(), herbList.end(), [](Herb herb1, Herb herb2) {
                return herb1.startingCost.GetMills() < herb2.startingCost.GetMills();
                });
            break;
        }
    }
    else if (filterType == 1) {
        switch (index) {
        case 1:
            // use lambda functions to define sorting behaviour for each desired sorting specifier
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name > herb2.name;
                });
            break;

        case 2:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category > herb2.category;
                });
            break;

        case 3:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal > herb2.currentStockTotal;
                });
            break;

        case 4:
            // doesnt work at all, rewrite
            std::sort(herbList.begin(), herbList.end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() > herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.startingBalance > herb2.startingBalance;
                });
            break;

        case 6:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.totalGramsPurchased > herb2.totalGramsPurchased;
                });
            break;

        case 7:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.totalGramsSold > herb2.totalGramsSold;
                });
            break;

        case 8:
            std::sort(herbList.begin(), herbList.end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier > herb2.preferredSupplier;
                });
            break;

        case 9:
            // same problem as above
            std::sort(herbList.begin(), herbList.end(), [](Herb herb1, Herb herb2) {
                return herb1.startingCost.GetMills() > herb2.startingCost.GetMills();
                });
            break;
        }
    }

    return &herbList;
}

bool HerbHandler::AddHerb(Herb newHerb)
{
    if (herbDBHandler.AddHerbToDB(newHerb)) {
        return true;
    }

    return false;
}

bool HerbHandler::DeleteHerb()
{
    return false;
}

bool HerbHandler::EditHerb()
{
    return false;
}