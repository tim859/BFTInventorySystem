#pragma once
#include <vector>
#include "DBHandler.h"
#include "Money.h"

class Formula;

class FormulaHandler
{
public:
	FormulaHandler();
	std::vector<Formula>* GetAllFormulas();
	Herb GetHerb(int rowID);
	void EditFormula();
	void DeleteFormula();

	// create formula functionality
	void AddHerbToFormula(Herb formulaHerb, int formulaAmount);
	void RemoveHerbFromFormula(int formulaIndex);
	void ClearHerbsFromFormula();
	bool AddFormula(std::string patientName, Money& costOfHerbs, Money& costToPatient);
	Money RecalculateCostOfHerbs();
	Money RecalculateCostToPatient();

private:
	std::vector<Formula> formulaList;

	// create formula functionality
	std::vector<Herb>* herbsInNewFormula;
	std::vector<int> amountsInNewFormula;
};

