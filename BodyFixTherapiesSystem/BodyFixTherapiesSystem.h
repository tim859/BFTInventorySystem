#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include <qsettings.h>
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
    void GoToMainMenu();
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

    // slots for add herb
    void AddHerb();

    // slots for edit herb
    void EditHerb();
    void DeleteHerb();
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

    // slots for settings
    void SaveSettings();
    void ChooseDatabase();
    void CloneDatabase();
    void DeleteAllHerbsAndFormulas();
    void DeleteAllFormulas();

private:
    // general properties and functions
    Ui::BodyFixTherapiesSystemClass ui;
    QSettings settings; // used to get and set values in config.ini
    std::string GetCurrentPageName();
    bool CheckForValidMonetaryValue(const std::string& input);
    bool CheckForValidStockAmount(const std::string& input);
    bool CheckForValidPercentage(const std::string& input);

    // herb properties and functions
    HerbHandler herbHandler;
    std::vector<Herb>* currentHerbListInMHTable;
    void UpdateHerbTable(std::vector<Herb>* herbList, QTableWidget* table);
    void ClearHerbFields();
    bool CheckForChangesInHerb();

    // formula properties and functions
    FormulaHandler formulaHandler;
    std::vector<Formula>* fullFormulaList;
    std::vector<Formula>* currentFormulaListInMFTable;
    std::vector<Herb> herbsToBeEdited; // herbs that have had their stock changed in the active herb list but should only have changes commmited to the db when the user commits those changes
    std::vector<Herb>* currentHerbListInCFAllHerbsTable;
    std::vector<Herb>* currentHerbListInEFAllHerbsTable;
    void UpdateMFTable(std::vector<Formula>* formulaList);
    bool CheckForChangesInFormula();
    void ClearFormulaFields();
    void UpdateHerbCosts();
    /// <summary>
    /// Sets contents of a herbs-in-formula table to the herb names and amounts currently stored in the active formula lists contained within formulaHandler
    /// </summary>
    void UpdateHerbsInFormulaTable();
};
