#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include "ui_BodyFixTherapiesSystem.h"
#include "HerbHandler.h"
#include "FormulaHandler.h"
#include "Formula.h"

class BodyFixTherapiesSystem : public QWidget
{
    Q_OBJECT

public:
    BodyFixTherapiesSystem(QWidget *parent = nullptr);
    ~BodyFixTherapiesSystem();

private slots:
    // slots for main menu
    void GoToManageHerbs();
    void GoToManageFormulas();
    //void GoToManageSuppliers();
    void GoToSettings();
    void QuitApp();

    // slots for manage herbs
    void SearchAndSortMHTable();
    void GoToAddHerb();
    void GoToEditHerb();
    void BackFromAddHerb();

    // slots for edit herbs
    void GoToEditHerbStock();
    void AddStockOfHerb();
    void ReduceStockOfHerb();
    void BackFromEditHerb();

    // slots for manage formulas
    void GoToCreateFormula();
    void GoToEditFormula();

    // slots for create formula and edit formula
    void SearchAndSortAllHerbsTable();
    void AddHerbToFormula();
    void RemoveHerbFromFormula();
    void FinishFormula();
    void BackToManageFormulas();
    void AddAmountToHerbInFormula();
    void RemoveAmountFromHerbInFormula();
    void BackFromEditFormula();

    // slots for create formula only
    void AbandonFormula();

    // slots for edit formula only
    void ConfirmEditFormula();
    void DeleteFormula();
    //void EditFormulaBack();

    // slots used by multiple pages
    void GoToMainMenu();

private:
    // general properties and functions
    Ui::BodyFixTherapiesSystemClass ui;
    std::string GetCurrentPageName();
    bool CheckForValidMonetaryValue(const std::string& input);
    bool CheckForValidStockAmount(const std::string& input);

    // manage herb properties and functions
    HerbHandler herbHandler;
    std::vector<Herb>* currentHerbListInMHTable;
    void UpdateHerbTable(std::vector<Herb>* herbList, QTableWidget* table);
    void AddHerb();
    void EditHerb();
    void DeleteHerb();
    void ClearHerbFields();
    bool CheckForChangesInHerb();

    // manage formula properties and functions
    FormulaHandler formulaHandler;
    std::vector<Formula>* fullFormulaList;
    std::vector<Formula>* currentFormulaListInMFTable;
    void UpdateMFTable(std::vector<Formula>* formulaList);

    // create formula and edit formula properties and functions
    std::vector<Herb> herbsToBeEdited; // herbs that have had their stock changed in the active herb list but should only have changes commmited to the db when the user commits those changes
    bool CheckForChangesInFormula();
    void ClearFormulaFields();
    void UpdateHerbCosts();
    /// <summary>
    /// Sets contents of a herbs-in-formula table to the herb names and amounts currently stored in the active formula lists contained within formulaHandler
    /// </summary>
    void UpdateHerbsInFormulaTable();

    // create formula properties and functions
    std::vector<Herb>* currentHerbListInCFAllHerbsTable;

    // edit formula properties and functions
    std::vector<Herb>* currentHerbListInEFAllHerbsTable;
};
