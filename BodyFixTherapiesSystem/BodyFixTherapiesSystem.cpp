#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qmessagebox.h>
#include <QInputDialog>
#include "BodyFixTherapiesSystem.h"
#include "DBHandler.h"
#include "Herb.h"
#include "Formula.h"
#include "Money.h"

BodyFixTherapiesSystem::BodyFixTherapiesSystem(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    // H1 font
    QFont h1("Calibri", 40);
    h1.setBold(true);

    // H2 font
    QFont h2("Calibri", 30);
    h2.setBold(true);

    ui.lblMMBFTInventorySystem->setFont(h1);
    ui.lblMMBFTInventorySystem->setStyleSheet("color: #3c829f");

    ui.lblMMMainMenu->setFont(h2);
    ui.lblMMMainMenu->setStyleSheet("color: #3c829f");

    QStringList herbTableHeaderLabels; 
    herbTableHeaderLabels << "Name" << "Category" << "Current Stock Level" << "Cost Per Gram" << "Preferred Supplier";
    QStringList formulaTableHeaderLabels;
    formulaTableHeaderLabels << "Patient Name" << "Cost of Herbs" << "Cost to Patient" << "List of Herbs in Formula";

    // connect ui buttons/signals to their respective functions/slots
    // ---------- main menu ----------
    connect(ui.btnMMManageHerbs, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);
    connect(ui.btnMMManageFormulas, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageFormulas);
    connect(ui.btnMMManageSuppliers, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageSuppliers);
    connect(ui.btnMMSettings, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToSettings);
    connect(ui.btnMMQuit, &QPushButton::clicked, this, &BodyFixTherapiesSystem::QuitApp);

    // ---------- manage herbs ----------
    connect(ui.btnMHAddHerb, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToAddHerb);
    connect(ui.btnMHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToMainMenu);
    ui.comboBoxMHSort->addItems({ "- None -", "Name", "Category", "Current stock level", "Cost per gram", "Preferred supplier" });
    connect(ui.comboBoxMHSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::SearchAndSortMHTable);
    connect(ui.radioBtnMHAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortMHTable);
    connect(ui.radioBtnMHDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortMHTable);
    connect(ui.btnMHSearch, &QPushButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortMHTable);
    connect(ui.lineEditMHSearchBar, &QLineEdit::returnPressed, this, &BodyFixTherapiesSystem::SearchAndSortMHTable);
    //connect(ui.btnMHReset, &QPushButton::clicked, this, &BodyFixTherapiesSystem::ResetMHTable);
    connect(ui.tableMHHerbTable, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::GoToEditHerb);
    connect(ui.btnMHEditHerb, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditHerb);

    // set custom column labels for herb table in manage herbs
    ui.tableMHHerbTable->setHorizontalHeaderLabels(herbTableHeaderLabels);

    // ---------- add herb ----------
    connect(ui.btnAHAddHerbToDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerb);
    connect(ui.btnAHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);

    // ---------- edit herb ----------
    connect(ui.btnEHEditHerbInDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::EditHerb);
    connect(ui.btnEHDeleteHerbFromDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::DeleteHerb);
    connect(ui.btnEHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);

    // ---------- manage formulas ----------
    connect(ui.btnMFCreateNewFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToCreateFormula);
    connect(ui.btnMFEditFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditFormula);
    connect(ui.btnMFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToMainMenu);

    // set custom column labels for formula table in manage formulas
    ui.tableMFFormulaTable->setHorizontalHeaderLabels(formulaTableHeaderLabels);

    // ---------- create formula -----------
    connect(ui.btnCFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageFormulas);
    ui.comboBoxCFSort->addItems({ "- None -", "Name", "Category", "Current stock level", "Cost per gram", "Preferred supplier" });
    connect(ui.comboBoxCFSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable);
    connect(ui.radioBtnCFAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable);
    connect(ui.radioBtnCFDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable);
    connect(ui.btnCFSearch, &QPushButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable);
    connect(ui.lineEditCFSearch, &QLineEdit::returnPressed, this, &BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable);
    connect(ui.tableCFHerbsInDatabase, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.btnCFAddHerbToFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.tableCFHerbsInFormula, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnCFRemoveHerbFromFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnCFFinishFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::FinishFormula);
    connect(ui.btnCFAbandonFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AbandonFormula);

    //set custom column labels for herb tables in create formula
    ui.tableCFHerbsInDatabase->setHorizontalHeaderLabels(herbTableHeaderLabels);
    QStringList herbAndAmount;
    herbAndAmount << "Name" << "Amount";
    ui.tableCFHerbsInFormula->setHorizontalHeaderLabels(herbAndAmount);

    // ---------- edit formula ----------
    connect(ui.btnEFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageFormulas);

    // ---------- manage suppliers ----------

    // ---------- settings ----------

    // stop user from being able to edit tables by clicking on them
    ui.tableMHHerbTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableMFFormulaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCFHerbsInDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCFHerbsInFormula->setEditTriggers(QAbstractItemView::NoEditTriggers);
    GoToMainMenu();    
}

BodyFixTherapiesSystem::~BodyFixTherapiesSystem()
{}

// ################################# Manage Herbs Functions ###################################################

void BodyFixTherapiesSystem::UpdateHerbTable(std::vector<Herb>* herbList, QTableWidget* herbTable) {
    herbTable->clearContents();
    herbTable->setRowCount(0);

    std::string stockTotalWithUnit;

    // populate table by getting information from list of herb structs
    // iterate through number of records in database
    for (int i = 0; i < herbList->size(); i++) {
        // add new row for next record
        herbTable->insertRow(herbTable->rowCount());
        // iterate through number of columns in database
        for (int j = 0; j < herbTable->columnCount(); j++) {

            switch (j) {
            case 0:
                herbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].name.c_str()));
                break;

            case 1:
                herbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].category.c_str()));
                break;

            case 2:
                stockTotalWithUnit = std::to_string((*herbList)[i].currentStockTotal) + "g";
                herbTable->setItem(i, j, new QTableWidgetItem(stockTotalWithUnit.c_str()));
                break;

            case 3:
                herbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].costPerGram.ToString().c_str()));
                break;

            case 4:
                herbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].preferredSupplier.c_str()));
                break;
            }
        }
    }
    // make contents of herbTable fill up the entire table space horizontally
    herbTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // resize columns to ensure no information is cut off by thin columns
    herbTable->resizeColumnsToContents();
}

void BodyFixTherapiesSystem::SearchAndSortMHTable()
{
    int filterType = 0;
    if (ui.radioBtnMHDescending->isChecked()) {
        filterType = 1;
    }

    std::vector<Herb>* searchedAndFilteredHerbList = herbHandler.GetSearchedAndSortedHerbs(fullHerbList, ui.lineEditMHSearchBar->text().toStdString(), ui.comboBoxMHSort->currentIndex(), filterType);
    UpdateHerbTable(searchedAndFilteredHerbList, ui.tableMHHerbTable);
    currentHerbListInMHTable = searchedAndFilteredHerbList;
}

void BodyFixTherapiesSystem::AddHerb()
{
    // check for empty fields
    if (ui.lineEditAHName->text().isEmpty() ||
        ui.lineEditAHCategory->text().isEmpty() ||
        ui.lineEditAHCurrentStockTotal->text().isEmpty() ||
        ui.lineEditAHCostPerGram->text().isEmpty() ||
        ui.lineEditAHPreferredSupplier->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "At least one field is empty");
        return;
    }

    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Add herb to database, are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {

        if (herbHandler.AddHerb(ui.lineEditAHName->text().toStdString(), 
            ui.lineEditAHCategory->text().toStdString(), 
            ui.lineEditAHCurrentStockTotal->text().toInt(), 
            ui.lineEditAHCostPerGram->text().toDouble(), 
            ui.lineEditAHPreferredSupplier->text().toStdString())) {

            QMessageBox::information(this, "Success", "Herb added to database successfully.");

            ui.lineEditAHName->clear();
            ui.lineEditAHCategory->clear();
            ui.lineEditAHCurrentStockTotal->clear();
            ui.lineEditAHCostPerGram->clear();
            ui.lineEditAHPreferredSupplier->clear();
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to add herb to database.");
        }
    }
}

void BodyFixTherapiesSystem::EditHerb()
{
    // check for empty fields
    if (ui.lineEditEHName->text().isEmpty() ||
        ui.lineEditEHCategory->text().isEmpty() ||
        ui.lineEditEHCurrentStockTotal->text().isEmpty() ||
        ui.lineEditEHCostPerGram->text().isEmpty() ||
        ui.lineEditEHPreferredSupplier->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "At least one field is empty");
        return;
    }

    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Edit herb fields in database, are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        if (herbHandler.EditHerb(editHerbID, ui.lineEditEHName->text().toStdString(),
            ui.lineEditEHCategory->text().toStdString(),
            ui.lineEditEHCurrentStockTotal->text().toInt(),
            ui.lineEditEHCostPerGram->text().toDouble(),
            ui.lineEditEHPreferredSupplier->text().toStdString())) {

            QMessageBox::information(this, "Success", "Herb edited in database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to edit herb in database.");
        }
    }
}

void BodyFixTherapiesSystem::DeleteHerb()
{
    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Delete the herb from the database, are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        if (herbHandler.DeleteHerb(editHerbID)) {
            QMessageBox::information(this, "Success", "Herb deleted from database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to delete herb from database.");
        }
    }
}

// manage herbs navigation buttons

void BodyFixTherapiesSystem::GoToManageHerbs() {
    ui.gridStackedWidget->setCurrentIndex(1);
    fullHerbList = herbHandler.GetAllHerbs();
    UpdateHerbTable(fullHerbList, ui.tableMHHerbTable);
    currentHerbListInMHTable = fullHerbList;
}

void BodyFixTherapiesSystem::GoToEditHerb()
{
    int selectedRow = ui.tableMHHerbTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected for editing.");
        return;
    }

    Herb editHerb = (*currentHerbListInMHTable)[selectedRow];
    editHerbID = editHerb.id;

    ui.gridStackedWidget->setCurrentIndex(6);

    // populate edit herb page with herb information
    ui.lineEditEHName->setText(QString::fromStdString(editHerb.name));
    ui.lineEditEHCategory->setText(QString::fromStdString(editHerb.category));
    ui.lineEditEHCurrentStockTotal->setText(QString::number(editHerb.currentStockTotal));
    ui.lineEditEHCostPerGram->setText(QString::number(editHerb.costPerGram.ToDouble()));
    ui.lineEditEHPreferredSupplier->setText(QString::fromStdString(editHerb.preferredSupplier));
}

// ###################################### Manage Formulas functions #####################################################

void BodyFixTherapiesSystem::UpdateMFTable(std::vector<Formula>* formulaList)
{
    ui.tableMFFormulaTable->clearContents();
    ui.tableMFFormulaTable->setRowCount(0);

    // populate table by getting information from list of formula structs
    // iterate through number of records in database
    for (int i = 0; i < formulaList->size(); i++) {
        // add new row for next record
        ui.tableMFFormulaTable->insertRow(ui.tableMFFormulaTable->rowCount());
        // iterate through number of columns in database
        for (int j = 0; j < ui.tableMFFormulaTable->columnCount(); j++) {

            switch (j) {
            case 0:
                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem((*formulaList)[i].patientName.c_str()));
                break;

            case 1:
                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem((*formulaList)[i].costOfHerbs.ToString().c_str()));
                break;

            case 2:
                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem((*formulaList)[i].costToPatient.ToString().c_str()));
                break;

            case 3:
                // create std::string list of herbs and amounts from the information stored in database
                // format e.g. Example Herb: 23g, Example Herb 2: 41g e.t.c.
                std::vector<Herb>* herbList = (*formulaList)[i].listOfHerbs;
                std::vector<int> herbAmountList = (*formulaList)[i].listOfHerbAmounts;
                std::string herbsAndAmountsListAsString;

                for (int k = 0; k < herbList->size(); k++) {
                    herbsAndAmountsListAsString += (*herbList)[k].name + ": " + std::to_string(herbAmountList[k]) + "g";
                    if (k < herbList->size() - 1) {
                        herbsAndAmountsListAsString += ", "; // Add a comma and space if it's not the last herb
                    }
                }

                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem(herbsAndAmountsListAsString.c_str()));
                break;
            }
        }
    }
    // make contents of table fill up the entire table space horizontally
    ui.tableMFFormulaTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // resize columns to ensure no information is cut off by thin columns
    ui.tableMFFormulaTable->resizeColumnsToContents();

    currentFormulaListInMFTable = formulaList;
}

void BodyFixTherapiesSystem::GoToManageFormulas() {
    ui.gridStackedWidget->setCurrentIndex(2);
    fullFormulaList = formulaHandler.GetAllFormulas();
    UpdateMFTable(fullFormulaList);
    currentFormulaListInMFTable = fullFormulaList;
}

void BodyFixTherapiesSystem::EditFormula()
{
}

void BodyFixTherapiesSystem::DeleteFormula()
{
}


// manage formulas navigation buttons

void BodyFixTherapiesSystem::GoToCreateFormula()
{
    ui.gridStackedWidget->setCurrentIndex(7);
    fullHerbList = herbHandler.GetAllHerbs();
    UpdateHerbTable(fullHerbList, ui.tableCFHerbsInDatabase);
    currentHerbListInCFAllHerbsTable = fullHerbList;
}

void BodyFixTherapiesSystem::GoToEditFormula()
{
    ui.gridStackedWidget->setCurrentIndex(8);
}

// ###################################### create formula functions ###########################################

void BodyFixTherapiesSystem::SearchAndSortCFAllHerbsTable() {
    int filterType = 0;
    if (ui.radioBtnCFDescending->isChecked()) {
        filterType = 1;
    }

    std::vector<Herb>* searchedAndFilteredHerbList = herbHandler.GetSearchedAndSortedHerbs(fullHerbList, ui.lineEditCFSearch->text().toStdString(), ui.comboBoxCFSort->currentIndex(), filterType);
    UpdateHerbTable(searchedAndFilteredHerbList, ui.tableCFHerbsInDatabase);
    currentHerbListInCFAllHerbsTable = searchedAndFilteredHerbList;
}

void BodyFixTherapiesSystem::AddHerbToFormula()
{
    int selectedRow = ui.tableCFHerbsInDatabase->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected to add to formula.");
        return;
    }

    Herb formulaHerb = (*currentHerbListInCFAllHerbsTable)[selectedRow];

    bool ok;
    int inputValue = QInputDialog::getInt(ui.pageCreateFormula,
        "Amount needed",
        // default value, min value, max value, step value
        "Please enter the amount of the herb you are using, in grams:", 0, 0, 1000, 1, &ok);

    if (ok) {
        int newRow = ui.tableCFHerbsInFormula->rowCount();
        ui.tableCFHerbsInFormula->insertRow(newRow);

        ui.tableCFHerbsInFormula->setItem(newRow, 0, new QTableWidgetItem(formulaHerb.name.c_str()));

        std::string herbAmount = std::to_string(inputValue) + "g";
        ui.tableCFHerbsInFormula->setItem(newRow, 1, new QTableWidgetItem(herbAmount.c_str()));

        // make contents of table fill up the entire table space horizontally
        ui.tableCFHerbsInFormula->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // resize columns to ensure no information is cut off by thin columns
        ui.tableCFHerbsInFormula->resizeColumnsToContents();

        formulaHandler.AddHerbToFormula(formulaHerb, inputValue);
        UpdateHerbCostsInCreateFormula();
    }
}

void BodyFixTherapiesSystem::RemoveHerbFromFormula()
{
    int selectedRow = ui.tableCFHerbsInFormula->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected to remove from formula.");
        return;
    }

    // remove row from table (ui only)
    ui.tableCFHerbsInFormula->removeRow(selectedRow);
    // remove herb from list(s) stored in formulaHandler object
    formulaHandler.RemoveHerbFromFormula(selectedRow);
    // update costs associated with the formula herb list
    UpdateHerbCostsInCreateFormula();
}

void BodyFixTherapiesSystem::FinishFormula() {
    // input validation - check for missing fields
    if (ui.lineEditCFPatientName->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Please input patient name before finishing formula.");
        return;
    }

    if (ui.tableCFHerbsInFormula->rowCount() <= 0) {
        QMessageBox::critical(this, "Error", "Please input at least one herb before finishing formula.");
        return;
    }

    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Finish the formula and add it to the database. Are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        Money costOfHerbs = formulaHandler.RecalculateCostOfHerbs();
        Money costToPatient = formulaHandler.RecalculateCostToPatient();
        if (formulaHandler.AddFormula(ui.lineEditCFPatientName->text().toStdString(), costOfHerbs, costToPatient)) {
            QMessageBox::information(this, "Success", "Formula successfully added to database.");
            // clear lineedit and formula table as well as reset labels
            ui.tableCFHerbsInFormula->clear();
            ui.tableCFHerbsInFormula->setRowCount(0);
            ui.lineEditCFPatientName->clear();
            ui.lblCFCostOfHerbs->setText("Cost of Herbs: \u00A300.00");
            ui.lblCFCostToPatient->setText("Cost to Patient: \u00A300.00");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to add formula to database.");
        }
    }
}

void BodyFixTherapiesSystem::AbandonFormula() {
    if (ui.lineEditCFPatientName->text().isEmpty() && ui.tableCFHerbsInFormula->rowCount() <= 0) {
        QMessageBox::critical(this, "Error", "You may not abandon that which does not yet exist.");
        return;
    }

    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Are you sure you want to abandon this formula?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        formulaHandler.ClearHerbsFromFormula();
        ui.tableCFHerbsInFormula->clear();
        ui.tableCFHerbsInFormula->setRowCount(0);
        ui.lineEditCFPatientName->clear();
        ui.lblCFCostOfHerbs->setText("Cost of Herbs: \u00A300.00");
        ui.lblCFCostToPatient->setText("Cost to Patient: \u00A300.00");
    }
}

void BodyFixTherapiesSystem::UpdateHerbCostsInCreateFormula()
{
    std::string costOfHerbs = "Cost of Herbs: " + formulaHandler.RecalculateCostOfHerbs().ToString();
    std::string costToPatient = "Cost to Patient: " + formulaHandler.RecalculateCostToPatient().ToString();

    ui.lblCFCostOfHerbs->setText(QString::fromStdString(costOfHerbs));
    ui.lblCFCostToPatient->setText(QString::fromStdString(costToPatient));
}

// ###################################### inline navigation buttons ###########################################
inline void BodyFixTherapiesSystem::GoToMainMenu() { ui.gridStackedWidget->setCurrentIndex(0); }
inline void BodyFixTherapiesSystem::GoToManageSuppliers() { ui.gridStackedWidget->setCurrentIndex(3); }
inline void BodyFixTherapiesSystem::GoToSettings() { ui.gridStackedWidget->setCurrentIndex(4); }
inline void BodyFixTherapiesSystem::GoToAddHerb() { ui.gridStackedWidget->setCurrentIndex(5); }
inline void BodyFixTherapiesSystem::QuitApp() { close(); }