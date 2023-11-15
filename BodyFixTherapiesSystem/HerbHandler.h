#pragma once
#include <vector>
// the class will completely break if this include declaration is removed. no clue why but thats fine it can just be left here
#include "DBHandler.h"
#include "Herb.h"

class Money;

class HerbHandler
{
public:
	HerbHandler();
	std::vector<Herb>* GetAllHerbs();
	std::vector<Herb>* GetSearchedHerbs(std::vector<Herb>* herbListToBeSearched, std::string searchString);
	// pass in index from combobox, for second value pass in 0 for ascending order and 1 for descending order
	std::vector<Herb>* GetSortedHerbs(std::vector<Herb>* herbListToBeSorted, int index, int sortType);
	// might be better to switch to passing in text from combobox in the future

	std::vector<Herb>* GetSearchedAndSortedHerbs(std::vector<Herb>* herbListToBeSearchedAndSorted, std::string searchString, int index, int sortType);
	bool AddHerb(std::string name, std::string category, int currentStockTotal, double costPerGram, std::string preferredSupplier);
	bool EditHerb(int rowID, std::string newName, std::string newCategory, int newCurrentStockTotal, Money newCostPerGram, std::string newPreferredSupplier);
	bool EditHerb(Herb editedHerb);
	bool DeleteHerb(int rowID);
	/// <summary>
	/// Increases currentStockTotal of the corresponding herb as well as calculating a new weighted average cost per gram of that herb
	/// </summary>
	Herb AddStockOfHerb(int rowID, int addAmount, double addCostPerGram);
	Herb ReduceStockOfHerb(int rowID, int reduceAmount);
	void SetLastDBAccurateHerb(Herb newLastDBAccurateHerb);
	Herb GetLastDBAccurateHerb();
	std::string GetHexColourForStockAmount(int stockAmount);

private:
	// previous method:
	// 1. get all columns in all records from the herbs table
	// 2. create a vector of herb objects based on this data
	// 3. send that vector to the ui class to be presented in the table
	// this is inefficient and bad because we are reloading ALL the records into memory every time the table is refreshed
	// and almost all the time, only one record has changed, meaning that almost all that computational time (which is clearly noticeable to the user) is wasted
	// 
	// i propose a different solution that should be implemented asap it will be as follows:
	// 1. on program start, get all columns in all records from the herb table
	// 2. populate herbList with Herb objects built from this data
	// 3. when a change is made to a herb in the database (add, edit, delete), instead of pulling all the records from the database, we simply add an item, edit an item or delete an item from herbList
	// 4. then, when the ui class requests an update of the herb info, we send a pointer to the freshly updated herbList. In this way, we don't need to be constantly pulling unnessecary info from the database.
	// there will be essentially no delay when refreshing a table instead of the current 1 second delay that there is with 1000 herbs.
	// update: delay is caused by populating a QTableWidget with data instead of anything to do with the database. Still, it's good to optimise this as much as possible.
	std::vector<Herb>* activeHerbList;
	Herb lastDBAccurateHerb;
};