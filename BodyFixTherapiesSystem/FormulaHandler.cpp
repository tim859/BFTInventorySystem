#include "FormulaHandler.h"
#include "Herb.h"
#include "Formula.h"
#include <iostream>
#include <QtSql/qsqlerror.h>

FormulaHandler::FormulaHandler() : 
    // for debug
    settings("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/config.ini", QSettings::IniFormat)

    // for release
    // settings("./config.ini", QSettings::IniFormat)
{
    herbsInActiveFormula = new std::vector<Herb>;
    lastDBAccurateFormula = Formula();
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

        
        std::string herbListString = formulaQuery.value(2).toString().toStdString();
        std::vector<Herb>* listOfHerbs = new std::vector<Herb>();
        std::vector<int> listofHerbAmounts = std::vector<int>();

        // check that the herbListString for the formula is not empty
        if (herbListString != "e") {

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
        }        

        formulaList.emplace_back(Formula(formulaQuery.value(0).toInt(), 
            formulaQuery.value(1).toString().toStdString(),
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
        Money(herbQuery.value(3).toDouble()),
        herbQuery.value(4).toString().toStdString());
}

void FormulaHandler::AddHerbToActiveFormula(Herb formulaHerb, int formulaAmount)
{

    // check for whether herb is already in formula
    for (int i = 0; i < herbsInActiveFormula->size(); i++) {
        if (formulaHerb == (*herbsInActiveFormula)[i]){
            AddAmountToHerbInActiveFormula(i, formulaAmount);
            return;
        }
    }
    // add herb to active formula lists
    herbsInActiveFormula->push_back(formulaHerb);
    herbAmountsInActiveFormula.push_back(formulaAmount);
}

void FormulaHandler::RemoveHerbFromActiveFormula(int herbIndex)
{
    herbsInActiveFormula->erase(herbsInActiveFormula->begin() + herbIndex);
    herbAmountsInActiveFormula.erase(herbAmountsInActiveFormula.begin() + herbIndex);
}

void FormulaHandler::AddAmountToHerbInActiveFormula(int herbIndex, int herbChangeAmount)
{
    herbAmountsInActiveFormula[herbIndex] += herbChangeAmount;
}

void FormulaHandler::RemoveAmountOfHerbInActiveFormula(int herbIndex, int herbChangeAmount)
{
    herbAmountsInActiveFormula[herbIndex] -= herbChangeAmount;
}

void FormulaHandler::ClearHerbsFromActiveFormula()
{
    herbsInActiveFormula->clear();
    herbAmountsInActiveFormula.clear();
}

bool FormulaHandler::AddFormula(std::string patientName)
{
    // doesn't matter what rowid value is set to here as DBHandler does not use it when adding a record and it is about to be set to the value that sqlite decides upon adding the formula to the DB
    Formula newFormula(0, patientName, herbsInActiveFormula, herbAmountsInActiveFormula);

    // add formula to database
    if (DBHandler::GetInstance().AddFormulaToDB(newFormula)) {
        // change new formulas rowid to what sqlite db set it to be
        newFormula.rowID = DBHandler::GetInstance().GetRowIDOfLastRecordInFormulaTable();

        // add formula to formula list
        formulaList.push_back(newFormula);

        ClearHerbsFromActiveFormula();
        return true;
    }
    return false;
}

bool FormulaHandler::EditFormula(int rowID, std::string newPatientName)
{
    Formula editedFormula(rowID, newPatientName, herbsInActiveFormula, herbAmountsInActiveFormula);

    // edit formula in database
    if (DBHandler::GetInstance().EditFormulaInDB(editedFormula)) {
        // edit formula in formula list and update the variable used for checking whether changes have been made to the formula or not
        for (int i = 0; i < formulaList.size(); i++) {
            if (formulaList[i].rowID == rowID) {
                formulaList[i] = editedFormula;
                lastDBAccurateFormula = editedFormula;
            }
        }
        return true;
    }
    return false;
}

bool FormulaHandler::DeleteFormula()
{
    // remove formula from database
    if (DBHandler::GetInstance().DeleteFormulaFromDB(lastDBAccurateFormula.rowID)) {
        // remove formula from formula list
        for (int i = 0; i < formulaList.size(); i++) {
            if (formulaList[i].rowID == lastDBAccurateFormula.rowID) {
                formulaList.erase(formulaList.begin() + i);
            }
        }
        return true;
    }
    return false;
}

Money FormulaHandler::RecalculateCostOfHerbs()
{
    Money totalAmount;
    for (int i = 0; i < herbsInActiveFormula->size(); i++) {
        Money herbCostForAmount = Money((*herbsInActiveFormula)[i].costPerGram * herbAmountsInActiveFormula[i]);
        //std::cout << "Total amount: " << totalAmount << "\nHerb cost for amount: " << herbCostForAmount;
        totalAmount += herbCostForAmount;
        //std::cout << "\nTotal amount + herb cost for amount = " << totalAmount << "\n\n";
    }

    //std::cout << "\ncost of herbs: " << totalAmount.ToString() << "\n";
    return totalAmount;
}

Money FormulaHandler::RecalculateCostToPatient()
{
    // can be redone to be more complex later when it actually matters
    const int extraAmount = 10000;
    Money totalAmount = RecalculateCostOfHerbs() + extraAmount;
    //std::cout << "cost to patient: " << totalAmount.ToString() << "\n";
    return totalAmount;
}

Money FormulaHandler::GetCostOfHerbsInFormula(int formulaIndex)
{
    Money costOfHerbs;
    Formula formula = formulaList[formulaIndex];
    for (int i = 0; i < formula.listOfHerbs->size(); i++) {
        Money herbCostForAmount = Money((*formula.listOfHerbs)[i].costPerGram * formula.listOfHerbAmounts[i]);
        costOfHerbs += herbCostForAmount;
    }
    return costOfHerbs;
}

Money FormulaHandler::GetCostToPatientOfFormula(int formulaIndex)
{
    double multiplier = (1 + (settings.value("SETTINGS/HerbCostPercentageAdded", 0).toDouble() / 100.0));

    return (Money(GetCostOfHerbsInFormula(formulaIndex) * multiplier));
}

std::vector<Herb>* FormulaHandler::GetHerbsInActiveFormula()
{
    return herbsInActiveFormula;
}

std::vector<int> FormulaHandler::GetHerbAmountsInActiveFormula()
{
    return herbAmountsInActiveFormula;
}

void FormulaHandler::SetLastDBAccurateFormula(Formula newEditFormula)
{
    lastDBAccurateFormula = newEditFormula;
}

Formula FormulaHandler::GetLastDBAccurateFormula()
{
    return lastDBAccurateFormula;
}

bool FormulaHandler::RemoveHerbFromFormulas(int deletedHerbRowID)
{
    // iterate through the list of formulas
    for (int i = 0; i < formulaList.size(); i++) {
        // iterate through the herb lists stored by each formula
        for (int j = 0; j < formulaList[i].listOfHerbs->size(); j++) {
            // check for a matching rowid to the deleted herb
            if ((*formulaList[i].listOfHerbs)[j].rowID == deletedHerbRowID) {

                // check for if we are removing the last herb in a formula, if so then clear the lists instead.
                if (formulaList[i].listOfHerbs->size() <= 1) {
                    formulaList[i].listOfHerbs->clear();
                    formulaList[i].listOfHerbAmounts.clear();
                }
                else {
                    // remove the deleted herb from the list of herbs
                    formulaList[i].listOfHerbs->erase(formulaList[i].listOfHerbs->begin() + j);
                    // remove the corresponding amount from the list of amounts
                    formulaList[i].listOfHerbAmounts.erase(formulaList[i].listOfHerbAmounts.begin() + j);
                }

                // update the herb list string of the formula in the database
                if (!DBHandler::GetInstance().EditFormulaInDB(formulaList[i])) {
                    return false;
                }
            }
        }
    }

    return true;
}
