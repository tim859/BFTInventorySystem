#pragma once
#include <vector>
#include "DBHandler.h"
#include "Money.h"
#include "Formula.h"

class FormulaHandler
{
public:
	FormulaHandler();

	std::vector<Formula>* GetAllFormulas();
	Herb GetHerb(int rowID);
	bool AddFormula(std::string patientName);
	bool EditFormula(int rowID, std::string newPatientName);
	bool DeleteFormula();	
	void AddHerbToActiveFormula(Herb formulaHerb, int formulaAmount);
	void RemoveHerbFromActiveFormula(int herbIndex);
	void AddAmountToHerbInActiveFormula(int herbIndex, int herbChangeAmount);
	void RemoveAmountOfHerbInActiveFormula(int herbIndex, int herbChangeAmount);
	void ClearHerbsFromActiveFormula();
	Money RecalculateCostOfHerbs();
	Money RecalculateCostToPatient();
	Money GetCostOfHerbsInFormula(int formulaIndex);
	Money GetCostToPatientOfFormula(int formulaIndex);
	std::vector<Herb>* GetHerbsInActiveFormula();
	std::vector<int> GetHerbAmountsInActiveFormula();
	void SetLastDBAccurateFormula(Formula newLastDBAccurateFormula);
	Formula GetLastDBAccurateFormula();
	bool RemoveHerbFromFormulas(int deletedHerbRowID);

private:
	// general formula properties and functions
	std::vector<Formula> formulaList;

	// create formula and edit formula properties and functions
	// vectors of the herbs and amounts that are currently in the tableHerbsInFormula
	std::vector<Herb>* herbsInActiveFormula;
	std::vector<int> herbAmountsInActiveFormula;

	// edit formula properties and functions
	// formula that is used to see what changes have been made by the user
	Formula lastDBAccurateFormula;

};

