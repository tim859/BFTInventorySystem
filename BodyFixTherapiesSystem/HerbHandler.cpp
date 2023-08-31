#include "HerbHandler.h"
#include "Herb.h"
#include <iostream>

HerbHandler::HerbHandler()
{
    GetAllHerbs();
}

std::vector<Herb>* HerbHandler::GetAllHerbs() {

    herbList.clear();
	QSqlQuery herbQuery = DBHandler::GetInstance().GetAllHerbsFromDB();

    while (herbQuery.next()) {
        herbList.emplace_back(Herb(herbQuery.value(0).toInt(),
            herbQuery.value(1).toString().toStdString(),
            herbQuery.value(2).toString().toStdString(),
            herbQuery.value(3).toInt(),
            Money(herbQuery.value(4).toInt(), 1),
            herbQuery.value(5).toString().toStdString()));
    }

    return &herbList;
}

std::vector<Herb>* HerbHandler::GetSearchedHerbs(std::vector<Herb>* herbListToBeSearched, std::string searchString)
{
    std::vector<Herb>* searchedHerbList = new std::vector<Herb>;

    for (int i = 0; i < herbListToBeSearched->size(); i++) {
        // Convert both the name and the search string to lowercase for case-insensitive comparison
        std::string herbNameLower = (*herbListToBeSearched)[i].name;
        std::transform(herbNameLower.begin(), herbNameLower.end(), herbNameLower.begin(), ::tolower);
        std::string searchStringLower = searchString;
        std::transform(searchStringLower.begin(), searchStringLower.end(), searchStringLower.begin(), ::tolower);

        // Check if the herb name contains the search string as a substring
        if (herbNameLower.find(searchStringLower) != std::string::npos) {
            searchedHerbList->push_back((*herbListToBeSearched)[i]);
        }
    }

    return searchedHerbList;
}

std::vector<Herb>* HerbHandler::GetFilteredHerbs(std::vector<Herb>* herbListToBeFiltered, int index, int filterType)
{
    if (filterType == 0) {
        switch (index) {
        case 1:
            // use lambda functions to define sorting behaviour for each desired sorting specifier
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name < herb2.name;
                });
            break;

        case 2:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category < herb2.category;
                });
            break;

        case 3:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal < herb2.currentStockTotal;
                });
            break;

        case 4:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() < herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier < herb2.preferredSupplier;
                });
            break;
        }
    }
    else if (filterType == 1) {
        switch (index) {
        case 1:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.name > herb2.name;
                });
            break;

        case 2:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.category > herb2.category;
                });
            break;

        case 3:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.currentStockTotal > herb2.currentStockTotal;
                });
            break;

        case 4:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](Herb herb1, Herb herb2) {
                return herb1.costPerGram.GetMills() > herb2.costPerGram.GetMills();
                });
            break;

        case 5:
            std::sort(herbListToBeFiltered->begin(), herbListToBeFiltered->end(), [](const Herb& herb1, const Herb& herb2) {
                return herb1.preferredSupplier > herb2.preferredSupplier;
                });
            break;
        }
    }

    return herbListToBeFiltered;
}

std::vector<Herb>* HerbHandler::GetSearchedAndSortedHerbs(std::vector<Herb>* herbListToBeSearchedAndFiltered, std::string searchString, int index, int filterType)
{
    if (searchString.empty() && index == 0) {
        // neither search or filter
        return herbListToBeSearchedAndFiltered;
    }
    if (searchString.empty()) {
        // filter only
        return GetFilteredHerbs(herbListToBeSearchedAndFiltered, index, filterType);
    }
    else if (index == 0) {
        // search only
        return GetSearchedHerbs(herbListToBeSearchedAndFiltered, searchString);
    }

    // search and filter
    std::vector<Herb>* searchedAndFilteredHerbList;
    searchedAndFilteredHerbList = GetSearchedHerbs(herbListToBeSearchedAndFiltered, searchString);
    searchedAndFilteredHerbList = GetFilteredHerbs(searchedAndFilteredHerbList, index, filterType);
    return searchedAndFilteredHerbList;
}

bool HerbHandler::AddHerb(std::string name, std::string category, int currentStockTotal, double costPerGram, std::string preferredSupplier)
{
    Herb newHerb(DBHandler::GetInstance().GetRowsInHerbTable() + 1, name, category, currentStockTotal, Money(costPerGram), preferredSupplier);

    if (DBHandler::GetInstance().AddHerbToDB(newHerb)) {
        herbList.push_back(newHerb);
        return true;
    }

    return false;
}

bool HerbHandler::EditHerb(int rowID, std::string newName, std::string newCategory, int newCurrentStockTotal, double newCostPerGram, std::string newPreferredSupplier)
{
    Herb editedHerb(rowID, newName, newCategory, newCurrentStockTotal, Money(newCostPerGram), newPreferredSupplier);

    if (DBHandler::GetInstance().EditHerbInDB(editedHerb)) {
        if (!herbList.empty()) {
            herbList[rowID - 1] = editedHerb;
        }
        return true;
    }

    return false;
}

bool HerbHandler::DeleteHerb(int rowID)
{
    if (DBHandler::GetInstance().DeleteHerbFromDB(rowID)) {
        if (rowID >= 0 && rowID < herbList.size()) {
            herbList.erase(herbList.begin() + rowID);
        }
        return true;
    }

    return false;
}
