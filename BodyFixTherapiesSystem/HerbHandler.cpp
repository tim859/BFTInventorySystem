#include "HerbHandler.h"
#include "Herb.h"
#include "Money.h"
#include <iostream>

HerbHandler::HerbHandler() :
    // for debug
    settings("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/config.ini", QSettings::IniFormat)

    // for release
    // settings("./config.ini", QSettings::IniFormat)
{
    RefreshHerbsFromDatabase();
}

std::vector<Herb>* HerbHandler::GetAllHerbs() {
    return activeHerbList;
}

void HerbHandler::RefreshHerbsFromDatabase()
{
    activeHerbList = new std::vector<Herb>;
    QSqlQuery herbQuery = DBHandler::GetInstance().GetAllHerbsFromDB();

    while (herbQuery.next()) {
        activeHerbList->emplace_back(Herb(herbQuery.value(0).toInt(),
            herbQuery.value(1).toString().toStdString(),
            herbQuery.value(2).toString().toStdString(),
            herbQuery.value(3).toInt(),
            Money(herbQuery.value(4).toDouble()),
            herbQuery.value(5).toString().toStdString()));
    }

    lastDBAccurateHerb = Herb();
}

std::vector<Herb>* HerbHandler::GetSearchedHerbs(std::vector<Herb>* herbListToBeSearched, std::string searchString)
{
    std::vector<Herb>* searchedHerbList = new std::vector<Herb>;

    for (int i = 0; i < herbListToBeSearched->size(); i++) {
        // convert both the name and the search string to lowercase for case-insensitive comparison
        std::string herbNameLower = (*herbListToBeSearched)[i].name;
        std::transform(herbNameLower.begin(), herbNameLower.end(), herbNameLower.begin(), ::tolower);
        std::string searchStringLower = searchString;
        std::transform(searchStringLower.begin(), searchStringLower.end(), searchStringLower.begin(), ::tolower);

        // check if the herb name contains the search string as a substring
        if (herbNameLower.find(searchStringLower) != std::string::npos) {
            searchedHerbList->push_back((*herbListToBeSearched)[i]);
        }
    }

    return searchedHerbList;
}

std::vector<Herb>* HerbHandler::GetSortedHerbs(std::vector<Herb>* herbListToBeSorted, int index, int sortType)
{
    if (sortType == 0) {
        switch (index) {
        case 1:
            // use lambda functions to define sorting behaviour for each desired sorting specifier
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name < herb2.name;
                });
            break;

        case 2:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category < herb2.category;
                });
            break;

        case 3:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal < herb2.currentStockTotal;
                });
            break;

        case 4:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() < herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier < herb2.preferredSupplier;
                });
            break;
        }
    }
    else if (sortType == 1) {
        switch (index) {
        case 1:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name > herb2.name;
                });
            break;

        case 2:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category > herb2.category;
                });
            break;

        case 3:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal > herb2.currentStockTotal;
                });
            break;

        case 4:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() > herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbListToBeSorted->begin(), herbListToBeSorted->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier > herb2.preferredSupplier;
                });
            break;
        }
    }

    return herbListToBeSorted;
}

std::vector<Herb>* HerbHandler::GetSearchedAndSortedHerbs(std::vector<Herb>* herbListToBeSearchedAndSorted, std::string searchString, int index, int sortType)
{
    if (searchString.empty() && index == 0) {
        // neither search or sort
        return herbListToBeSearchedAndSorted;
    }
    if (searchString.empty()) {
        // sort only
        return GetSortedHerbs(herbListToBeSearchedAndSorted, index, sortType);
    }
    else if (index == 0) {
        // search only
        return GetSearchedHerbs(herbListToBeSearchedAndSorted, searchString);
    }

    // search and filter
    std::vector<Herb>* searchedAndSortedHerbList;
    searchedAndSortedHerbList = GetSearchedHerbs(herbListToBeSearchedAndSorted, searchString);
    searchedAndSortedHerbList = GetSortedHerbs(searchedAndSortedHerbList, index, sortType);
    return searchedAndSortedHerbList;
}

bool HerbHandler::AddHerb(std::string name, std::string category, int currentStockTotal, double costPerGram, std::string preferredSupplier)
{
    // doesn't matter what rowid value is set to here as DBHandler does not use it when adding a record and it is about to be set to the value that sqlite decides upon adding the herb to the DB
    Herb newHerb(0, name, category, currentStockTotal, Money(costPerGram), preferredSupplier);

    // add herb to database
    if (DBHandler::GetInstance().AddHerbToDB(newHerb)) {
        // change new herbs rowid to what sqlite db set it to be
        newHerb.rowID = DBHandler::GetInstance().GetRowIDOfLastRecordInHerbTable();

        // add herb to herb list
        activeHerbList->push_back(newHerb);
        return true;
    }
    return false;
}

bool HerbHandler::EditHerb(int rowID, std::string newName, std::string newCategory, int newCurrentStockTotal, Money newCostPerGram, std::string newPreferredSupplier)
{
    Herb editedHerb(rowID, newName, newCategory, newCurrentStockTotal, newCostPerGram, newPreferredSupplier);

    // edit herb in database
    if (DBHandler::GetInstance().EditHerbInDB(editedHerb)) {
        // edit herb in herb list
        for (int i = 0; i < activeHerbList->size(); i++) {
            if ((*activeHerbList)[i].rowID == rowID) {
                (*activeHerbList)[i] = editedHerb;
            }
        }
        return true;
    }
    return false;
}

bool HerbHandler::EditHerb(Herb editedHerb)
{
    // edit herb in database
    if (DBHandler::GetInstance().EditHerbInDB(editedHerb)) {
        // edit herb in herb list
        for (int i = 0; i < activeHerbList->size(); i++) {
            if ((*activeHerbList)[i].rowID == editedHerb.rowID) {
                (*activeHerbList)[i] = editedHerb;
            }
        }
        return true;
    }
    return false;
}

bool HerbHandler::DeleteHerb(int rowID)
{
    // remove herb from database
    if (DBHandler::GetInstance().DeleteHerbFromDB(rowID)) {
        // remove herb from herb list
        for (int i = 0; i < activeHerbList->size(); i++) {
            if ((*activeHerbList)[i].rowID == rowID) {
                activeHerbList->erase(activeHerbList->begin() + i);
            }
        }
        return true;
    } 
    return false;
}

Herb HerbHandler::AddStockOfHerb(int rowID, int addStockAmount, double addCostPerGram)
{
    Herb* editedHerb = nullptr;
    for (int i = 0; i < activeHerbList->size(); i++) {
        if ((*activeHerbList)[i].rowID == rowID) {
            editedHerb = &(*activeHerbList)[i];
            break;
        }
    }

    // editedHerb should hopefully never be null but the null check stops the ide complaining
    if (editedHerb == nullptr) {
        std::cout << "nullptr in AddStockOfHerb";
        return Herb();
    }

    // weighted average cost per gram formula:     
    // weightedPreviousCost = previousStock * previousAverageCostPerGram
    // weightedNewCost = addedStockAmount * addedCostPerGram
    // newTotalStock = previousStock + addedStockAmount
    // newAverageCost = (weightedPreviousCost + weightedNewCost) / newTotalStock

    // 1st test - example calculation with current stock of 750 at 0.5/gram and new amount of 250 at 1/gram:     
    // 750 * 0.5 = 375
    // 250 * 1 = 250
    // 750 + 250 = 1000
    //(375 + 250) / 1000 = 0.625 aka £0.62
    // PASSED

    // 2nd test - example calculation with current stock of 682 at 0.96/gram and new amount of 1333 at 1.43/gram:
    // 682 * 0.96 = 654.72
    // 1333 * 1.43 = 1906.19
    // 682 + 1333 = 2015
    // (654.72 + 1906.19) / 2015 = 1.270923077 aka £1.27
    // PASSED

    double weightedPreviousCost = editedHerb->costPerGram.ToDouble() * editedHerb->currentStockTotal;
    double weightedNewCost = addStockAmount * addCostPerGram;
    editedHerb->currentStockTotal += addStockAmount;
    Money newWeightedAverageCost = Money((weightedPreviousCost + weightedNewCost) / editedHerb->currentStockTotal);
    editedHerb->costPerGram = newWeightedAverageCost;

    return *editedHerb;
}

Herb HerbHandler::ReduceStockOfHerb(int rowID, int reduceAmount)
{
    // finds the relevant herb in the active herb list
    Herb* editedHerb = nullptr;
    for (int i = 0; i < activeHerbList->size(); i++) {
        if ((*activeHerbList)[i].rowID == rowID) {
            editedHerb = &(*activeHerbList)[i];
            break;
        }
    }

    // checks for nullptr
    if (editedHerb == nullptr) {
        std::cout << "nullptr in ReduceStockOfHerb";
        return Herb();
    }

    // checks for trying to reduce herb stock to less than 0
    if (reduceAmount > editedHerb->currentStockTotal) {
        return Herb();
    }

    editedHerb->currentStockTotal -= reduceAmount;

    return *editedHerb;
}

void HerbHandler::SetLastDBAccurateHerb(Herb newLastDBAccurateHerb)
{
    lastDBAccurateHerb = newLastDBAccurateHerb;
}

Herb HerbHandler::GetLastDBAccurateHerb()
{
    return lastDBAccurateHerb;
}

std::string HerbHandler::GetHexColourForStockAmount(int stockAmount)
{
    if (stockAmount <= settings.value("SETTINGS/VeryLowOrNoStock", 0).toInt()) {
        return "#610000"; // dark red
    }
    else if (stockAmount <= settings.value("SETTINGS/LowStock", 0).toInt()) {
        return "#99431f"; // dark orange
    }
    else if (stockAmount <= settings.value("SETTINGS/MediumStock", 0).toInt()) {
        return "#957f07"; // dark yellow
    }
    else {
        return "#154f30"; // dark green
    }
}

std::string HerbHandler::GetTotalHerbStockValue()
{
    return std::string();
}
