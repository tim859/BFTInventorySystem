#pragma once
#include <string>
#include <vector>
#include "Money.h"
#include "Herb.h"

struct Formula {

	Formula() : rowID(0), patientName(""), listOfHerbs(new std::vector<Herb>), listOfHerbAmounts() {}

	Formula(int initRowID, std::string initPatientName,std::vector<Herb>* initListOfHerbs, std::vector<int> initListOfHerbAmounts)
		: rowID(initRowID), patientName(initPatientName),listOfHerbs(initListOfHerbs), listOfHerbAmounts(initListOfHerbAmounts) {}

	int rowID;
	std::string patientName;
	std::vector<Herb>* listOfHerbs;
	std::vector<int> listOfHerbAmounts;

	bool operator== (Formula other);
	friend std::ostream& operator<< (std::ostream& os, Formula& obj);
};