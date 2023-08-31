#pragma once
#include <string>
#include "Money.h"
#include "Herb.h"

struct Formula {

	Formula(int initID, std::string initPatientName, Money initCostOfHerbs, Money initCostToPatient, std::vector<Herb>* initListOfHerbs, std::vector<int> initListOfHerbAmounts)
		: id(initID), patientName(initPatientName), costOfHerbs(initCostOfHerbs), costToPatient(initCostToPatient), listOfHerbs(initListOfHerbs), listOfHerbAmounts(initListOfHerbAmounts) {}

	int id;
	std::string patientName;
	Money costOfHerbs;
	Money costToPatient;
	std::vector<Herb>* listOfHerbs;
	std::vector<int> listOfHerbAmounts;
};