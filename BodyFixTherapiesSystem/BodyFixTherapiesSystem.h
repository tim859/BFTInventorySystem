#pragma once

#include <QtWidgets/QWidget>
#include <iostream>
#include "ui_BodyFixTherapiesSystem.h"
#include "HerbHandler.h"

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
    void GoToAddHerb();
    void UpdateMHTable(std::vector<Herb>* herbList);
    void FilterMHTable();

    // slots used by multiple pages
    void GoToMainMenu();

private:
    Ui::BodyFixTherapiesSystemClass ui;
    HerbHandler herbHandler;
    void AddHerb();
};
