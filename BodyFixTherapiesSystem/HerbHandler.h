#pragma once
#include <vector>
#include "HerbDBHandler.h"

class Herb;

/* Class for handling interactions between GUI and HerbDBHandler classes. All interactions and calculation involving herbs should use this class*/
class HerbHandler
{
public:
	HerbHandler();
	std::vector<Herb>* GetAllHerbs();
	std::vector<Herb>* GetSearchedHerbs(std::vector<Herb>* herbListToBeSearched, std::string searchString);
	// pass in index from combobox, for second value pass in 0 for ascending order and 1 for descending order
	std::vector<Herb>* GetFilteredHerbs(std::vector<Herb>* herbListToBeFiltered, int index, int filterType);
	// might be better to switch to passing in text from combobox in the future

	std::vector<Herb>* GetSearchedAndFilteredHerbs(std::vector<Herb>* herbListToBeSearchedAndFiltered, std::string searchString, int index, int filterType);
	bool AddHerb(std::string name, std::string category, int currentStockTotal, double costPerGram, std::string preferredSupplier);
	bool EditHerb(int rowID, std::string newName, std::string newCategory, int newCurrentStockTotal, double newCostPerGram, std::string newPreferredSupplier);
	bool DeleteHerb(int rowID);

private:
	std::vector<Herb> herbList;
	HerbDBHandler herbDBHandler;
};