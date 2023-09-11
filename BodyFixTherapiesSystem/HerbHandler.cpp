#include "HerbHandler.h"
#include "Herb.h"
#include <iostream>

HerbHandler::HerbHandler()
{
    herbList = new std::vector<Herb>;
    QSqlQuery herbQuery = DBHandler::GetInstance().GetAllHerbsFromDB();

    while (herbQuery.next()) {
        herbList->emplace_back(Herb(herbQuery.value(0).toInt(),
            herbQuery.value(1).toString().toStdString(),
            herbQuery.value(2).toString().toStdString(),
            herbQuery.value(3).toInt(),
            Money(herbQuery.value(4).toDouble()),
            herbQuery.value(5).toString().toStdString()));
    }
}

std::vector<Herb>* HerbHandler::GetAllHerbs() {
    return herbList;
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
        herbList->push_back(newHerb);
        return true;
    }
    return false;
}

bool HerbHandler::EditHerb(int rowID, std::string newName, std::string newCategory, int newCurrentStockTotal, double newCostPerGram, std::string newPreferredSupplier)
{
    Herb editedHerb(rowID, newName, newCategory, newCurrentStockTotal, Money(newCostPerGram), newPreferredSupplier);

    // edit herb in database
    if (DBHandler::GetInstance().EditHerbInDB(editedHerb)) {
        // edit herb in herb list
        for (int i = 0; i < herbList->size(); i++) {
            if ((*herbList)[i].rowID == rowID) {
                (*herbList)[i] = editedHerb;
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
        for (int i = 0; i < herbList->size(); i++) {
            if ((*herbList)[i].rowID == rowID) {
                herbList->erase(herbList->begin() + i);
            }
        }
        return true;
    } 
    return false;
}
