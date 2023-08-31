#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include "ui_BodyFixTherapiesSystem.h"
#include "HerbHandler.h"
#include "FormulaHandler.h"

class Formula;

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
    void GoToManageSuppliers();
    void GoToSettings();
    void QuitApp();

    // slots for manage herbs
    void SearchAndSortMHTable();
    void GoToAddHerb();
    void GoToEditHerb();

    // slots for manage formulas
    void GoToCreateFormula();
    void GoToEditFormula();

    // slots for create formula
    void SearchAndSortCFAllHerbsTable();
    void AddHerbToFormula();
    void RemoveHerbFromFormula();
    void FinishFormula();
    void AbandonFormula();

    // slots used by multiple pages
    void GoToMainMenu();

private:
    Ui::BodyFixTherapiesSystemClass ui;

    // manage herb properties and functions
    HerbHandler herbHandler;
    std::vector<Herb>* fullHerbList;
    std::vector<Herb>* currentHerbListInMHTable;
    int editHerbID;
    void UpdateHerbTable(std::vector<Herb>* herbList, QTableWidget* table);
    void AddHerb();
    void EditHerb();
    void DeleteHerb();

    // manage formula properties and functions
    FormulaHandler formulaHandler;
    std::vector<Formula>* fullFormulaList;
    std::vector<Formula>* currentFormulaListInMFTable;
    int editFormulaID;
    void UpdateMFTable(std::vector<Formula>* formulaList);
    void EditFormula();
    void DeleteFormula();

    // create formula properties and functions
    std::vector<Herb>* currentHerbListInCFAllHerbsTable;
    void UpdateHerbCostsInCreateFormula();
};
