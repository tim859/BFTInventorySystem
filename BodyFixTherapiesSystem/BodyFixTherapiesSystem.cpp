#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qmessagebox.h>
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
    ui.comboBoxMHFilterCombo->addItems({ "- No filter -", "Name", "Category", "Current stock level", "Cost per gram", "Preferred supplier" });
    connect(ui.comboBoxMHFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::SearchAndFilterMHTable);
    connect(ui.radioBtnMHAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndFilterMHTable);
    connect(ui.radioBtnMHDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::SearchAndFilterMHTable);
    connect(ui.btnMHSearch, &QPushButton::clicked, this, &BodyFixTherapiesSystem::SearchAndFilterMHTable);
    connect(ui.lineEditMHSearchBar, &QLineEdit::returnPressed, this, &BodyFixTherapiesSystem::SearchAndFilterMHTable);
    //connect(ui.btnMHReset, &QPushButton::clicked, this, &BodyFixTherapiesSystem::ResetMHTable);
    connect(ui.tableMHHerbTable, &QTableWidget::itemDoubleClicked, this, &BodyFixTherapiesSystem::GoToEditHerb);
    connect(ui.btnMHEditHerb, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToEditHerb);

    // set custom column labels (herb database column titles)
    QStringList headerLabels;
    headerLabels << "Name" << "Category" << "Current Stock Level" << "Cost Per Gram" << "Preferred Supplier";
    ui.tableMHHerbTable->setHorizontalHeaderLabels(headerLabels);

    // stop user from being able to edit table by clicking on it
    ui.tableMHHerbTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ---------- add herb ----------
    connect(ui.btnAHAddHerbToDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerb);
    connect(ui.btnAHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);

    // ---------- edit herb ----------
    connect(ui.btnEHEditHerbInDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::EditHerb);
    connect(ui.btnEHDeleteHerbFromDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::DeleteHerb);
    connect(ui.btnEHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);

    // ---------- manage formulas ----------

    // ---------- manage suppliers ----------

    // ---------- settings ----------

    //UpdateMHTable(herbHandler.GetAllHerbs());
    GoToMainMenu();
}

BodyFixTherapiesSystem::~BodyFixTherapiesSystem()
{}

void BodyFixTherapiesSystem::UpdateMHTable(std::vector<Herb>* herbList) {
    ui.tableMHHerbTable->clearContents();
    ui.tableMHHerbTable->setRowCount(0);
    //ui.lblMHBFTInventorySystem->setText(QString::number(static_cast<qlonglong>(herbList->size())));

    std::string stockTotalWithUnit;
    std::string costPerGramWithUnit;

    // hide default row numbers (vertical header)
    //ui.tableMHHerbTable->verticalHeader()->setVisible(false);

    // populate table by getting information from list of herb structs
    // iterate through number of records in database
    for (int i = 0; i < herbList->size(); i++) {
        // add new row for next record
        ui.tableMHHerbTable->insertRow(ui.tableMHHerbTable->rowCount());
        // iterate through number of columns in database
        for (int j = 0; j < ui.tableMHHerbTable->columnCount(); j++) {

            switch (j) {
            case 0:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].name.c_str()));
                break;

            case 1:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].category.c_str()));
                break;

            case 2:
                stockTotalWithUnit = std::to_string((*herbList)[i].currentStockTotal) + "g";
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem(stockTotalWithUnit.c_str()));
                break;

            case 3:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].costPerGram.ToString().c_str()));
                break;

            case 4:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].preferredSupplier.c_str()));
                break;
            }
        }
    }
    // make contents of table fill up the entire table space horizontally
    ui.tableMHHerbTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // resize columns to ensure no information is cut off by thin columns
    ui.tableMHHerbTable->resizeColumnsToContents();

    currentHerbListInTable = herbList;
}

void BodyFixTherapiesSystem::SearchAndFilterMHTable()
{
    int filterType = 0;
    if (ui.radioBtnMHDescending->isChecked()) {
        filterType = 1;
    }

    std::vector<Herb>* searchedAndFilteredHerbList = herbHandler.GetSearchedAndFilteredHerbs(fullHerbList, ui.lineEditMHSearchBar->text().toStdString(), ui.comboBoxMHFilterCombo->currentIndex(), filterType);
    UpdateMHTable(searchedAndFilteredHerbList);
}

//void BodyFixTherapiesSystem::ResetMHTable()
//{
//    // empty search bar
//    ui.lineEditMHSearchBar->clear();
//    // set filter combo to 0
//    ui.comboBoxMHFilterCombo->setCurrentIndex(0);
//    UpdateMHTable(herbHandler.GetAllHerbs());
//}

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

// navigation buttons

void BodyFixTherapiesSystem::GoToManageHerbs() {
    ui.gridStackedWidget->setCurrentIndex(1);
    fullHerbList = herbHandler.GetAllHerbs();
    UpdateMHTable(fullHerbList);
}

void BodyFixTherapiesSystem::GoToEditHerb()
{
    int selectedRow = ui.tableMHHerbTable->currentRow();

    if (selectedRow < 0) {
        QMessageBox::critical(this, "Error", "No herb selected for editing.");
        return;
    }

    Herb editHerb = (*currentHerbListInTable)[selectedRow];
    editHerbID = editHerb.id;

    ui.gridStackedWidget->setCurrentIndex(6);

    // populate edit herb page with herb information
    ui.lineEditEHName->setText(QString::fromStdString(editHerb.name));
    ui.lineEditEHCategory->setText(QString::fromStdString(editHerb.category));
    ui.lineEditEHCurrentStockTotal->setText(QString::number(editHerb.currentStockTotal));
    ui.lineEditEHCostPerGram->setText(QString::number(editHerb.costPerGram.ToDouble()));
    ui.lineEditEHPreferredSupplier->setText(QString::fromStdString(editHerb.preferredSupplier));
}

inline void BodyFixTherapiesSystem::GoToMainMenu() { ui.gridStackedWidget->setCurrentIndex(0); }
inline void BodyFixTherapiesSystem::GoToManageFormulas() { ui.gridStackedWidget->setCurrentIndex(2); }
inline void BodyFixTherapiesSystem::GoToManageSuppliers() { ui.gridStackedWidget->setCurrentIndex(3); }
inline void BodyFixTherapiesSystem::GoToSettings() { ui.gridStackedWidget->setCurrentIndex(4); }
inline void BodyFixTherapiesSystem::GoToAddHerb() { ui.gridStackedWidget->setCurrentIndex(5); }
inline void BodyFixTherapiesSystem::QuitApp() { close(); }