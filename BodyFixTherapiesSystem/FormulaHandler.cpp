#include "FormulaHandler.h"
#include "Herb.h"
#include "Formula.h"
#include <iostream>
#include <qsqlerror.h>



FormulaHandler::FormulaHandler()
{
    herbsInNewFormula = new std::vector<Herb>;
}

std::vector<Formula>* FormulaHandler::GetAllFormulas()
{
	formulaList.clear();
    QSqlQuery formulaQuery = DBHandler::GetInstance().GetAllFormulasFromDB();

    while (formulaQuery.next()) {

        // read string in formulaQuery.value(4).toString().toStdString() which will be in format "h-a,h-a,h-a,h-ae"
        // the legend for this format is as follows:
        // 'h' is the rowid of a herb and the way to identify which herb it is
        // '-' is the seperator between a herb rowid and the amount of that herb
        // 'a' is the amount of the specified herb
        // ',' is the seperator between herbs and their respective amounts i.e. seeing a ',' means we are moving onto the next herb and its amount
        // 'e' is the end of the string and denotes the end of the herb/amount list
        // retrieve all herbs from database and create a list of herbs that corresponds to the list in the formula record
        // as well as a corresponding list of herb amounts that will match the indexes in the list of herbs
        // in this way, herbs and their corresponding amounts can be stored and retreived simply by using two seperate vectors
        // for this to work properly though, anything done to one list must ALWAYS be done to the other, being out of sync by just 1 index makes both lists useless

        std::vector<Herb>* listOfHerbs = new std::vector<Herb>(); // Allocate memory for the list of herbs
        std::vector<int> listofHerbAmounts = std::vector<int>();
        std::string herbListString = formulaQuery.value(4).toString().toStdString();

        std::string tempStringForHerbsAndAmounts;

        for (char character : herbListString) {
            // add herb to listOfHerbs based off rowID in temp string and clear temp string
            if (character == '-') {
                listOfHerbs->push_back(GetHerb(std::stoi(tempStringForHerbsAndAmounts)));
                tempStringForHerbsAndAmounts.clear();
            }
            // add amount to listOfHerbAmounts based off amount in temp string and clear temp string
            else if (character == ',') {
                listofHerbAmounts.push_back(std::stoi(tempStringForHerbsAndAmounts));
                tempStringForHerbsAndAmounts.clear();
            }
            // add amount to listOfHerbAmounts based off amount in temp string and break from loop as we've reached the end of the string
            else if (character == 'e') {
                listofHerbAmounts.push_back(std::stoi(tempStringForHerbsAndAmounts));
                break;
            }
            else {
                // add character to temp string for processing in other if cases
                tempStringForHerbsAndAmounts += character;
            }
        }

        formulaList.emplace_back(Formula(formulaQuery.value(0).toInt(), 
            formulaQuery.value(1).toString().toStdString(),
            Money(formulaQuery.value(2).toInt(), 1),
            Money(formulaQuery.value(3).toInt(), 1),
            listOfHerbs, 
            listofHerbAmounts));
    }

	return &formulaList;
}

Herb FormulaHandler::GetHerb(int rowID)
{
    QSqlQuery herbQuery = DBHandler::GetInstance().GetHerbFromDB(rowID);

    return Herb(rowID,
        herbQuery.value(0).toString().toStdString(),
        herbQuery.value(1).toString().toStdString(),
        herbQuery.value(2).toInt(),
        Money(herbQuery.value(3).toInt()),
        herbQuery.value(4).toString().toStdString());
}

void FormulaHandler::AddHerbToFormula(Herb formulaHerb, int formulaAmount)
{
    herbsInNewFormula->push_back(formulaHerb);
    amountsInNewFormula.push_back(formulaAmount);
}

void FormulaHandler::RemoveHerbFromFormula(int formulaIndex)
{
    herbsInNewFormula->erase(herbsInNewFormula->begin() + formulaIndex);
    amountsInNewFormula.erase(amountsInNewFormula.begin() + formulaIndex);
}

void FormulaHandler::ClearHerbsFromFormula()
{
    herbsInNewFormula->clear();
    amountsInNewFormula.clear();
}

bool FormulaHandler::AddFormula(std::string patientName, Money& costOfHerbs, Money& costToPatient)
{
    Formula newFormula(DBHandler::GetInstance().GetRowsInFormulaTable() + 1, patientName, costOfHerbs, costToPatient, herbsInNewFormula, amountsInNewFormula);

    if (DBHandler::GetInstance().AddFormulaToDB(newFormula)) {
        formulaList.push_back(newFormula);
        ClearHerbsFromFormula();
        return true;
    }

    return false;
}

void FormulaHandler::EditFormula()
{
}

void FormulaHandler::DeleteFormula()
{
}

Money FormulaHandler::RecalculateCostOfHerbs()
{
    Money totalAmount;
    for (int i = 0; i < herbsInNewFormula->size(); i++) {
        totalAmount += ((*herbsInNewFormula)[i].costPerGram * amountsInNewFormula[i]);
    }

    return totalAmount;
}

Money FormulaHandler::RecalculateCostToPatient()
{
    // can be redone to be more complex later when it actually matters
    const int extraAmount = 101;
    return RecalculateCostOfHerbs() + extraAmount;
}
