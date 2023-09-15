#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qmessagebox.h>
#include <QInputDialog>
#include <regex>
#include "BodyFixTherapiesSystem.h"
#include "DBHandler.h"
#include "Herb.h"
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
    QStringList herbAndAmount;
    herbAndAmount << "Name" << "Amount";

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
    connect(ui.btnEHEditStockOfHerb, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditHerbStock);
    connect(ui.btnEHEditHerbInDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::EditHerb);
    connect(ui.btnEHDeleteHerbFromDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::DeleteHerb);
    connect(ui.btnEHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);
    
    // ---------- edit herb stock ----------
    connect(ui.btnESHAddStock, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddStockOfHerb);
    connect(ui.btnESHReduceStock, &QPushButton::clicked, this, &BodyFixTherapiesSystem::ReduceStockOfHerb);
    connect(ui.btnESHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditHerb);

    // ---------- manage formulas ----------
    connect(ui.btnMFCreateNewFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToCreateFormula);
    connect(ui.tableMFFormulaTable, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::GoToEditFormula);
    connect(ui.btnMFEditFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditFormula);
    connect(ui.btnMFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToMainMenu);
    // set custom column labels for formula table in manage formulas
    ui.tableMFFormulaTable->setHorizontalHeaderLabels(formulaTableHeaderLabels);

    // ---------- create formula -----------
    ui.comboBoxCFSort->addItems({ "- None -", "Name", "Category", "Current stock level", "Cost per gram", "Preferred supplier" });
    connect(ui.comboBoxCFSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.radioBtnCFAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.radioBtnCFDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.btnCFSearch, &QPushButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.lineEditCFSearch, &QLineEdit::returnPressed, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.tableCFHerbsInDatabase, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.btnCFAddHerbToFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.tableCFHerbsInFormula, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnCFRemoveHerbFromFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnCFFinishFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::FinishFormula);
    connect(ui.btnCFAbandonFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AbandonFormula);
    connect(ui.btnCFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::BackToManageFormulas);
    //set custom column labels for herb tables in create formula
    ui.tableCFHerbsInDatabase->setHorizontalHeaderLabels(herbTableHeaderLabels);
    ui.tableCFHerbsInFormula->setHorizontalHeaderLabels(herbAndAmount);

    // ---------- edit formula ----------
    ui.comboBoxEFSort->addItems({ "- None -", "Name", "Category", "Current stock level", "Cost per gram", "Preferred supplier" });
    connect(ui.comboBoxEFSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.radioBtnEFAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.radioBtnEFDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.btnEFSearch, &QPushButton::clicked, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.lineEditEFSearch, &QLineEdit::returnPressed, this, &BodyFixTherapiesSystem::SearchAndSortAllHerbsTable);
    connect(ui.tableEFHerbsInDatabase, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.btnEFAddHerbToFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerbToFormula);
    connect(ui.tableEFHerbsInFormula, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnEFRemoveHerbFromFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::RemoveHerbFromFormula);
    connect(ui.btnEFConfirmEditToFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::ConfirmEditFormula);
    connect(ui.btnEFDeleteFormula, &QPushButton::clicked, this, &BodyFixTherapiesSystem::DeleteFormula);
    connect(ui.btnEFBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::BackToManageFormulas);
    //set custom column labels for herb tables in create formula
    ui.tableEFHerbsInDatabase->setHorizontalHeaderLabels(herbTableHeaderLabels);
    ui.tableEFHerbsInFormula->setHorizontalHeaderLabels(herbAndAmount);

    // ---------- manage suppliers ----------

    // ---------- settings ----------

    // stop user from being able to edit tables by clicking on them
    ui.tableMHHerbTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableMFFormulaTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCFHerbsInDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableCFHerbsInFormula->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableEFHerbsInDatabase->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableEFHerbsInFormula->setEditTriggers(QAbstractItemView::NoEditTriggers);
    GoToMainMenu();
}

BodyFixTherapiesSystem::~BodyFixTherapiesSystem()
{}

std::string BodyFixTherapiesSystem::GetCurrentPageName() {
    return ui.gridStackedWidget->currentWidget()->objectName().toStdString();
}

// use regular expressions to check for valid stock amounts and money values

bool BodyFixTherapiesSystem::CheckForValidStockAmount(const std::string& input) {
    std::regex pattern("^[0-9]+$");
    return std::regex_match(input, pattern);
}

bool BodyFixTherapiesSystem::CheckForValidMonetaryValue(const std::string& input)
{
    std::regex pattern("^\\d+\\.\\d{1,2}$");
    return std::regex_match(input, pattern);
}

// ############################################################################################################################
// ################################# Manage herbs functions ###################################################################
// ############################################################################################################################

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

    std::vector<Herb>* searchedAndFilteredHerbList = herbHandler.GetSearchedAndSortedHerbs(herbHandler.GetAllHerbs(), ui.lineEditMHSearchBar->text().toStdString(), ui.comboBoxMHSort->currentIndex(), filterType);
    UpdateHerbTable(searchedAndFilteredHerbList, ui.tableMHHerbTable);
    currentHerbListInMHTable = searchedAndFilteredHerbList;
}

void BodyFixTherapiesSystem::AddHerb()
{
    // check for empty fields
    if (ui.lineEditAHName->text().isEmpty() ||
        ui.lineEditAHCategory->text().isEmpty() ||
        ui.lineEditAHPreferredSupplier->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "At least one field is empty.");
        return;
    }

    // check for valid stock amount
    if (!CheckForValidStockAmount(ui.lineEditAHCurrentStockTotal->text().toStdString())) {
        QMessageBox::critical(this, "Error", "Please enter a valid stock amount.");
        return;
    }

    // check for valid monetary value
    if (!CheckForValidMonetaryValue(ui.lineEditAHCostPerGram->text().toStdString())) {
        QMessageBox::critical(this, "Error", "Please enter a valid monetary value.");
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
            GoToManageHerbs();
            QMessageBox::information(this, "Success", "Herb added to database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to add herb to database.");
        }
    }
}

void BodyFixTherapiesSystem::EditHerb()
{
    if (ui.lineEditAHName->text().isEmpty() ||
        ui.lineEditAHCategory->text().isEmpty() ||
        ui.lineEditAHPreferredSupplier->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "At least one field is empty.");
        return;
    }

    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Edit herb fields in database, are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        if (herbHandler.EditHerb(herbHandler.GetLastDBAccurateHerb().rowID,
            ui.lineEditEHName->text().toStdString(),
            ui.lineEditEHCategory->text().toStdString(),
            herbHandler.GetLastDBAccurateHerb().currentStockTotal, 
            herbHandler.GetLastDBAccurateHerb().costPerGram,
            ui.lineEditEHPreferredSupplier->text().toStdString())) {
            GoToManageHerbs();
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
        if (herbHandler.DeleteHerb(herbHandler.GetLastDBAccurateHerb().rowID)) {
            if (!formulaHandler.RemoveHerbFromFormulas(herbHandler.GetLastDBAccurateHerb().rowID)) {
                QMessageBox::critical(this, "Error", "Herb was successfully deleted but formulas with the deleted herb in them were not successfully updated.");
            }
            GoToManageHerbs();
            QMessageBox::information(this, "Success", "Herb deleted from database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to delete herb from database.");
        }
    }
}

void BodyFixTherapiesSystem::ClearHerbFields()
{
    // TODO: rework so it works with edit herb fields as well
    ui.lineEditAHName->clear();
    ui.lineEditAHCategory->clear();
    ui.lineEditAHCurrentStockTotal->clear();
    ui.lineEditAHCostPerGram->clear();
    ui.lineEditAHPreferredSupplier->clear();
}

// manage herbs navigation buttons

void BodyFixTherapiesSystem::GoToManageHerbs() {
    ClearHerbFields();
    ui.gridStackedWidget->setCurrentWidget(ui.pageManageHerbs);
    UpdateHerbTable(herbHandler.GetAllHerbs(), ui.tableMHHerbTable);
    currentHerbListInMHTable = herbHandler.GetAllHerbs();
}

void BodyFixTherapiesSystem::GoToEditHerb()
{
    int selectedRow = ui.tableMHHerbTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected for editing.");
        return;
    }

    Herb editHerb = (*currentHerbListInMHTable)[selectedRow];
    herbHandler.SetLastDBAccurateHerb(editHerb);

    ui.gridStackedWidget->setCurrentWidget(ui.pageEditHerb);

    // populate edit herb page with herb information
    ui.lineEditEHName->setText(QString::fromStdString(editHerb.name));
    ui.lineEditEHCategory->setText(QString::fromStdString(editHerb.category));
    ui.lblEHCurrentStockTotal->setText(QString::fromStdString("Current Stock Total: " + std::to_string(editHerb.currentStockTotal)));
    ui.lblEHCostPerGram->setText(QString::fromStdString("Average Cost Per Gram: " + editHerb.costPerGram.ToString()));
    ui.lineEditEHPreferredSupplier->setText(QString::fromStdString(editHerb.preferredSupplier));
}

// ############################################################################################################################
// ###################################### Edit Herbs functions ################################################################
// ############################################################################################################################

void BodyFixTherapiesSystem::GoToEditHerbStock()
{
    ui.gridStackedWidget->setCurrentWidget(ui.pageEditHerbStock);

    std::string currentStockTotal = "Current Stock Total: " + std::to_string(herbHandler.GetLastDBAccurateHerb().currentStockTotal) + "g";
    std::string averageCostPerGram = "Average Cost Per Gram: " + herbHandler.GetLastDBAccurateHerb().costPerGram.ToString();
    ui.lblESHCurrentStockTotal->setText(QString::fromStdString(currentStockTotal));
    ui.lblESHAverageCostPerGram->setText(QString::fromStdString(averageCostPerGram));
}

void BodyFixTherapiesSystem::AddStockOfHerb()
{
    // get amount of herb to be used in grams and store it in inputValue
    bool ok;
    // default value, min value, max value, step value
    int inputAmountValue = QInputDialog::getInt(this, "Amount needed", "Please enter the amount of the herb you are adding, in grams:", 1, 1, 10000, 1, &ok);
    if (!ok) {
        return;
    }
    // default value, min value, max value, number of decimal places
    double inputCostPerGram = QInputDialog::getDouble(this, "Value needed", "Please enter the cost per gram of the amount:", 0.01, 0.01, 10000.0, 2, &ok);
    if (!ok) {
        return;
    }

    Herb editedHerb = herbHandler.AddStockOfHerb(herbHandler.GetLastDBAccurateHerb().rowID, inputAmountValue, inputCostPerGram);

    // if AddStockOfHerb returns a Herb object with a rowid of 0, that means it failed to properly add the new stock of herb
    // see AddStockOfHerb implementation for the reasons why this could happen
    if (editedHerb.rowID != 0) {
        // update stock level and average cost per gram and display success window
        std::string currentStockTotal = "Current Stock Total: " + std::to_string(editedHerb.currentStockTotal) + "g";
        std::string averageCostPerGram = "Average Cost Per Gram: " + editedHerb.costPerGram.ToString();
        ui.lblESHCurrentStockTotal->setText(QString::fromStdString(currentStockTotal));
        ui.lblESHAverageCostPerGram->setText(QString::fromStdString(averageCostPerGram));
        QMessageBox::information(this, "Success", "Herb stock successfully updated.");
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to update herb stock.");
    }
}

void BodyFixTherapiesSystem::ReduceStockOfHerb()
{
    bool ok;
    int inputAmountValue = QInputDialog::getInt(this, "Amount needed", "Please enter the amount you are reducing the herb by, in grams:", 1, 1, 10000, 1, &ok);
    if (!ok) {
        return;
    }

    Herb editedHerb = herbHandler.ReduceStockOfHerb(herbHandler.GetLastDBAccurateHerb().rowID, inputAmountValue);

    if (editedHerb.rowID != 0) {
        std::string currentStockTotal = "Current Stock Total: " + std::to_string(editedHerb.currentStockTotal) + "g";
        ui.lblESHCurrentStockTotal->setText(QString::fromStdString(currentStockTotal));
        QMessageBox::information(this, "Success", "Herb stock successfully updated.");
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to update herb stock.");
    }
}

// ############################################################################################################################
// ###################################### Manage formulas functions ###########################################################
// ############################################################################################################################

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
                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem(formulaHandler.GetCostOfHerbsInFormula(i).ToString().c_str()));
                break;

            case 2:
                ui.tableMFFormulaTable->setItem(i, j, new QTableWidgetItem(formulaHandler.GetCostToPatientOfFormula(i).ToString().c_str()));
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
    ClearFormulaFields();
    ui.gridStackedWidget->setCurrentWidget(ui.pageManageFormulas);
    fullFormulaList = formulaHandler.GetAllFormulas();
    UpdateMFTable(fullFormulaList);
    currentFormulaListInMFTable = fullFormulaList;
}

// manage formulas navigation buttons

void BodyFixTherapiesSystem::GoToCreateFormula()
{
    formulaHandler.SetLastDBAccurateFormula(Formula());
    ui.gridStackedWidget->setCurrentWidget(ui.pageCreateFormula);
    UpdateHerbTable(herbHandler.GetAllHerbs(), ui.tableCFHerbsInDatabase);
    currentHerbListInCFAllHerbsTable = herbHandler.GetAllHerbs();
    formulaHandler.ClearHerbsFromActiveFormula();
}

void BodyFixTherapiesSystem::GoToEditFormula()
{
    int selectedRow = ui.tableMFFormulaTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No formula selected for editing.");
        return;
    }

    Formula editFormula = (*currentFormulaListInMFTable)[selectedRow];
    formulaHandler.SetLastDBAccurateFormula(editFormula);
    ui.gridStackedWidget->setCurrentWidget(ui.pageEditFormula);

    // populate edit formula page with formula information
    ui.lineEditEFPatientName->setText(QString::fromStdString(editFormula.patientName));
    std::string costOfHerbs = "Cost of Herbs: " + ui.tableMFFormulaTable->item(selectedRow, 1)->text().toStdString();
    ui.lblEFCostOfHerbs->setText(QString::fromStdString(costOfHerbs));
    std::string costToPatient = "Cost to Patient: " + ui.tableMFFormulaTable->item(selectedRow, 2)->text().toStdString();
    ui.lblEFCostToPatient->setText(QString::fromStdString(costToPatient));

    formulaHandler.ClearHerbsFromActiveFormula();
    // add herbs in formula to be edited, to the formula table in edit formula and then to the list in formulaHandler to ensure they can be propely dealt with
    for (int i = 0; i < editFormula.listOfHerbs->size(); i++) {
        int newRow = ui.tableEFHerbsInFormula->rowCount();
        ui.tableEFHerbsInFormula->insertRow(newRow);

        ui.tableEFHerbsInFormula->setItem(newRow, 0, new QTableWidgetItem((*editFormula.listOfHerbs)[i].name.c_str()));

        std::string herbAmount = std::to_string(editFormula.listOfHerbAmounts[i]) + "g";
        ui.tableEFHerbsInFormula->setItem(newRow, 1, new QTableWidgetItem(herbAmount.c_str()));

        formulaHandler.AddHerbToActiveFormula((*editFormula.listOfHerbs)[i], editFormula.listOfHerbAmounts[i]);
    }
    // make contents of table fill up the entire table space horizontally
    ui.tableEFHerbsInFormula->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // resize columns to ensure no information is cut off by thin columns
    ui.tableEFHerbsInFormula->resizeColumnsToContents();

    // update herbs in database table in edit formula
    UpdateHerbTable(herbHandler.GetAllHerbs(), ui.tableEFHerbsInDatabase);
    currentHerbListInEFAllHerbsTable = herbHandler.GetAllHerbs();
}

// ############################################################################################################################
// ###################################### Create formula and edit formula functions ###########################################
// ############################################################################################################################


void BodyFixTherapiesSystem::SearchAndSortAllHerbsTable() {
    int filterType = 0;
    // define pointers to all the ui widgets that will be used in the function
    QTableWidget* herbTable = nullptr;
    QLineEdit* searchLineEdit = nullptr;
    QRadioButton* descendingRadioBtn = nullptr;
    QComboBox* sortComboBox = nullptr;
    std::vector<Herb>* currentHerbListinAllHerbsTable;

    // assign pointers to all the relevant ui widgets based on the page that we are on
    // this exact process is done in every other function for the create/edit formula functionality
    if (GetCurrentPageName() == "pageCreateFormula") {
        herbTable = ui.tableCFHerbsInDatabase;
        searchLineEdit = ui.lineEditCFSearch;
        descendingRadioBtn = ui.radioBtnCFDescending;
        sortComboBox = ui.comboBoxCFSort;
        currentHerbListinAllHerbsTable = currentHerbListInCFAllHerbsTable;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        herbTable = ui.tableEFHerbsInDatabase;
        searchLineEdit = ui.lineEditEFSearch;
        descendingRadioBtn = ui.radioBtnEFDescending;
        sortComboBox = ui.comboBoxEFSort;
        currentHerbListinAllHerbsTable = currentHerbListInEFAllHerbsTable;
    }
    else {
        QMessageBox::critical(this, "Error", "Cannot search and sort a herb table on this page");
        return;
    }

    if (descendingRadioBtn->isChecked()) {
        filterType = 1;
    }

    std::vector<Herb>* searchedAndFilteredHerbList = herbHandler.GetSearchedAndSortedHerbs(herbHandler.GetAllHerbs(), searchLineEdit->text().toStdString(), sortComboBox->currentIndex(), filterType);
    UpdateHerbTable(searchedAndFilteredHerbList, herbTable);

    currentHerbListinAllHerbsTable = searchedAndFilteredHerbList;
}


void BodyFixTherapiesSystem::AddHerbToFormula()
{
    // TODO: quality of life fix - check for duplicate herbs already in the table and simply update the quantity of the already existing herb rather than having another of the same herb in the list

    QTableWidget* tableHerbsInDatabase = nullptr;
    QTableWidget* tableHerbsInFormula = nullptr;
    std::vector<Herb>* currentHerbListInAllHerbsTable = nullptr;

    if (GetCurrentPageName() == "pageCreateFormula") {
        tableHerbsInDatabase = ui.tableCFHerbsInDatabase;
        tableHerbsInFormula = ui.tableCFHerbsInFormula;
        currentHerbListInAllHerbsTable = currentHerbListInCFAllHerbsTable;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        tableHerbsInDatabase = ui.tableEFHerbsInDatabase;
        tableHerbsInFormula = ui.tableEFHerbsInFormula;
        currentHerbListInAllHerbsTable = currentHerbListInEFAllHerbsTable;
    }
    else {
        QMessageBox::critical(this, "Error", "Cannot add a herb to a formula on this page");
        return;
    }

    int selectedRow = tableHerbsInDatabase->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected to add to formula.");
        return;
    }

    Herb formulaHerb = (*currentHerbListInAllHerbsTable)[selectedRow];

    // get amount of herb to be used in grams and store it in inputValue
    bool ok;
    int inputValue = QInputDialog::getInt(this,
        "Amount needed",
        // default value, min value, max value, step value
        "Please enter the amount of the herb you are using, in grams:", 1, 1, 1000, 1, &ok);

    if (ok) {
        // check for duplicate herb built in to AddHerbToFormula function
        if (formulaHandler.AddHerbToActiveFormula(formulaHerb, inputValue)) {
            // check returns true - herb was not in formula already
            int newRow = tableHerbsInFormula->rowCount();
            tableHerbsInFormula->insertRow(newRow);

            tableHerbsInFormula->setItem(newRow, 0, new QTableWidgetItem(formulaHerb.name.c_str()));

            std::string herbAmount = std::to_string(inputValue) + "g";
            tableHerbsInFormula->setItem(newRow, 1, new QTableWidgetItem(herbAmount.c_str()));

            // make contents of table fill up the entire table space horizontally
            tableHerbsInFormula->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            // resize columns to ensure no information is cut off by thin columns
            tableHerbsInFormula->resizeColumnsToContents();
        }
        else {
            // check returns false - herb was already in formula and has had its quantity updated - now needs to be updated in the table as well
            std::vector<Herb>* currentHerbsInFormula = formulaHandler.GetHerbsInActiveFormula();
            for (int i = 0; i < currentHerbsInFormula->size(); i++) {
                if ((*currentHerbsInFormula)[i].rowID == formulaHerb.rowID) {
                    std::string herbAmount = std::to_string(formulaHandler.GetHerbAmountsInActiveFormula()[i]) + "g";
                    ui.tableEFHerbsInFormula->setItem(i, 1, new QTableWidgetItem(herbAmount.c_str()));
                }
            }
        }
        UpdateHerbCosts();
    }
}

void BodyFixTherapiesSystem::RemoveHerbFromFormula()
{
    QTableWidget* tableHerbsInFormula = nullptr;

    if (GetCurrentPageName() == "pageCreateFormula") {
        tableHerbsInFormula = ui.tableCFHerbsInFormula;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        tableHerbsInFormula = ui.tableEFHerbsInFormula;
    }
    else {
        QMessageBox::critical(this, "Error", "Cannot remove a herb from a formula on this page");
        return;
    }

    int selectedRow = tableHerbsInFormula->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected to remove from formula.");
        return;
    }

    // remove row from table (ui only)
    tableHerbsInFormula->removeRow(selectedRow);
    // remove herb from list(s) stored in formulaHandler object
    formulaHandler.RemoveHerbFromActiveFormula(selectedRow);
    // update costs associated with the formula herb list
    UpdateHerbCosts();
}

void BodyFixTherapiesSystem::UpdateHerbCosts()
{
    QLabel* lblCostOfHerbs = nullptr;
    QLabel* lblCostToPatient = nullptr;

    if (GetCurrentPageName() == "pageCreateFormula") {
        lblCostOfHerbs = ui.lblCFCostOfHerbs;
        lblCostToPatient = ui.lblCFCostToPatient;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        lblCostOfHerbs = ui.lblEFCostOfHerbs;
        lblCostToPatient = ui.lblEFCostToPatient;
    }
    else {
        QMessageBox::critical(this, "Error", "Cannot update herb costs on this page");
        return;
    }

    std::string costOfHerbs = "Cost of Herbs: " + formulaHandler.RecalculateCostOfHerbs().ToString();
    std::string costToPatient = "Cost to Patient: " + formulaHandler.RecalculateCostToPatient().ToString();

    lblCostOfHerbs->setText(QString::fromStdString(costOfHerbs));
    lblCostToPatient->setText(QString::fromStdString(costToPatient));
}

void BodyFixTherapiesSystem::ClearFormulaFields()
{
    QLineEdit* lineEditPatientName = nullptr;
    QTableWidget* tableHerbsInFormula = nullptr;
    QLabel* lblCostOfHerbs = nullptr;
    QLabel* lblCostToPatient = nullptr;

    if (GetCurrentPageName() == "pageCreateFormula") {
        lineEditPatientName = ui.lineEditCFPatientName;
        tableHerbsInFormula = ui.tableCFHerbsInFormula;
        lblCostOfHerbs = ui.lblCFCostOfHerbs;
        lblCostToPatient = ui.lblCFCostToPatient;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        lineEditPatientName = ui.lineEditEFPatientName;
        tableHerbsInFormula = ui.tableEFHerbsInFormula;
        lblCostOfHerbs = ui.lblEFCostOfHerbs;
        lblCostToPatient = ui.lblEFCostToPatient;
    }
    else {
        return;
    }

    tableHerbsInFormula->clear();
    tableHerbsInFormula->setRowCount(0);
    lineEditPatientName->clear();
    lblCostOfHerbs->setText("Cost of Herbs: \u00A300.00");
    lblCostToPatient->setText("Cost to Patient: \u00A300.00");
    formulaHandler.ClearHerbsFromActiveFormula();
}

bool BodyFixTherapiesSystem::CheckForChangesInFormula()
{
    QLineEdit* lineEditPatientName = nullptr;
    QTableWidget* tableHerbsInFormula = nullptr;

    if (GetCurrentPageName() == "pageCreateFormula") {
        lineEditPatientName = ui.lineEditCFPatientName;
        tableHerbsInFormula = ui.tableCFHerbsInFormula;
    }
    else if (GetCurrentPageName() == "pageEditFormula") {
        lineEditPatientName = ui.lineEditEFPatientName;
        tableHerbsInFormula = ui.tableEFHerbsInFormula;
    }
    else {
        QMessageBox::critical(this, "Error", "Cannot check for changes in formula on this page");
        return false;
    }

    // check for difference in patient name
    if (formulaHandler.GetLastDBAccurateFormula().patientName != lineEditPatientName->text().toStdString()) {
        return true;
    }

    // check for difference in the amount of herbs/herb amounts in the table
    if (formulaHandler.GetLastDBAccurateFormula().listOfHerbs->size() != tableHerbsInFormula->rowCount() ||
        formulaHandler.GetLastDBAccurateFormula().listOfHerbAmounts.size() != tableHerbsInFormula->rowCount()) {
        return true;
    }

    // check for difference in the values of the herb names and amounts
    for (int i = 0; i < tableHerbsInFormula->rowCount(); i++) {

        // remove the g suffix from the herb amount so it can be compared as an int
        std::string herbAmountString = tableHerbsInFormula->item(i, 1)->text().toStdString();
        herbAmountString.resize(herbAmountString.size() - 1);
        int herbAmountValue = std::stoi(herbAmountString);

        if ((*formulaHandler.GetLastDBAccurateFormula().listOfHerbs)[i].name != tableHerbsInFormula->item(i, 0)->text().toStdString() ||
            formulaHandler.GetLastDBAccurateFormula().listOfHerbAmounts[i] != herbAmountValue) {
            return true;
        }
    }
    return false;
}

void BodyFixTherapiesSystem::BackToManageFormulas()
{
    if (CheckForChangesInFormula()) {
        QMessageBox::critical(this, "Error", "Formula has unsaved changes, please either confirm and save your changes or abandon them before going back.");
        return;
    }

    GoToManageFormulas();
}

// ############################################################################################################################
// ####################################### Create formula functions only ######################################################
// ############################################################################################################################

void BodyFixTherapiesSystem::FinishFormula()
{
    // left it like this in case this function is used for multiple pages in the future
    QLineEdit* lineEditPatientName = ui.lineEditCFPatientName;
    QTableWidget* tableHerbsInFormula = ui.tableCFHerbsInFormula;
    QLabel* lblCostOfHerbs = ui.lblCFCostOfHerbs;
    QLabel* lblCostToPatient = ui.lblCFCostToPatient;

    // input validation - check for empty fields
    if (lineEditPatientName->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Please input patient name before finishing formula.");
        return;
    }

    if (tableHerbsInFormula->rowCount() <= 0) {
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
        if (formulaHandler.AddFormula(lineEditPatientName->text().toStdString())) {
            GoToManageFormulas();
            QMessageBox::information(this, "Success", "Formula successfully added to database.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to add formula to database.");
        }
    }
}

void BodyFixTherapiesSystem::AbandonFormula()
{
    QLineEdit* lineEditPatientName = ui.lineEditCFPatientName;
    QTableWidget* tableHerbsInFormula = ui.tableCFHerbsInFormula;
    QLabel* lblCostOfHerbs = ui.lblCFCostOfHerbs;
    QLabel* lblCostToPatient = ui.lblCFCostToPatient;

    // input validation - check for empty fields
    if (lineEditPatientName->text().isEmpty() && tableHerbsInFormula->rowCount() <= 0) {
        QMessageBox::critical(this, "Error", "You may not abandon that which does not yet exist.");
        return;
    }

    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Are you sure you want to abandon this formula?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        GoToManageFormulas();
    }
}

// ############################################################################################################################
// ####################################### Edit formula functions only ########################################################
// ############################################################################################################################

void BodyFixTherapiesSystem::ConfirmEditFormula()
{
    QLineEdit* lineEditPatientName = ui.lineEditEFPatientName;
    QTableWidget* tableHerbsInFormula = ui.tableEFHerbsInFormula;
    QLabel* lblCostOfHerbs = ui.lblEFCostOfHerbs;
    QLabel* lblCostToPatient = ui.lblEFCostToPatient;

    // input validation - check for empty fields
    if (lineEditPatientName->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Please input patient name before confirming edit of formula.");
        return;
    }

    if (tableHerbsInFormula->rowCount() <= 0) {
        QMessageBox::critical(this, "Error", "Please input at least one herb before confirming edit of formula.");
        return;
    }

    // if no changes have been made by the user then we don't want to make any changes to the database
    if (!CheckForChangesInFormula()) {
        QMessageBox::critical(this, "Error", "You cannot confirm editing the formula because no changes have been made yet.");
        return;
    }

    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Confirm editing the formula and change it in the database. Are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        Money costOfHerbs = formulaHandler.RecalculateCostOfHerbs();
        Money costToPatient = formulaHandler.RecalculateCostToPatient();
        if (formulaHandler.EditFormula(formulaHandler.GetLastDBAccurateFormula().rowID, lineEditPatientName->text().toStdString())) {
            GoToManageFormulas();
            QMessageBox::information(this, "Success", "Formula successfully edited in the database.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to edit formula in database.");
        }
    }
}

void BodyFixTherapiesSystem::DeleteFormula()
{
    // display a confirmation dialog
    QMessageBox confirmationBox;
    confirmationBox.setIcon(QMessageBox::Question);
    confirmationBox.setText("Delete the formula from the database, are you sure?");
    confirmationBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    int reply = confirmationBox.exec();

    if (reply == QMessageBox::Yes) {
        if (formulaHandler.DeleteFormula()) {
            GoToManageFormulas();
            QMessageBox::information(this, "Success", "Formula deleted from database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to delete formula from database.");
        }
    }
}

// ############################################################################################################################
// ###################################### Inline navigation buttons ###########################################################
// ############################################################################################################################

inline void BodyFixTherapiesSystem::GoToMainMenu() { ui.gridStackedWidget->setCurrentWidget(ui.pageMainMenu); }
inline void BodyFixTherapiesSystem::GoToManageSuppliers() { ui.gridStackedWidget->setCurrentWidget(ui.pageManageSuppliers); }
inline void BodyFixTherapiesSystem::GoToSettings() { ui.gridStackedWidget->setCurrentWidget(ui.pageSettings); }
inline void BodyFixTherapiesSystem::GoToAddHerb() { ui.gridStackedWidget->setCurrentWidget(ui.pageAddHerb); }
inline void BodyFixTherapiesSystem::QuitApp() { close(); }