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
	std::vector<Herb> SearchForHerbs();
	// pass in index from combobox, for second value pass in 0 for ascending order and 1 for descending order
	std::vector<Herb>* GetFilteredHerbs(int index, int filterType);
	// might be better to switch to passing in text from combobox in the future
	bool AddHerb(Herb newHerb);
	bool DeleteHerb();
	bool EditHerb();

private:
	std::vector<Herb> herbList;
	HerbDBHandler herbDBHandler;
};