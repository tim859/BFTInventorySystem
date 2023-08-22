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
    ui.comboBoxMHFilterCombo->addItems({ "- No filter -", "Name", "Category", "Current stock level", "Cost per gram", "Starting balance", "Total grams purchased", "Total grams sold", "Preferred supplier", "Starting cost" });
    connect(ui.comboBoxMHFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BodyFixTherapiesSystem::FilterMHTable);
    connect(ui.radioBtnMHAscending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::FilterMHTable);
    connect(ui.radioBtnMHDescending, &QRadioButton::clicked, this, &BodyFixTherapiesSystem::FilterMHTable);

    // set custom column labels (herb database column titles)
    QStringList headerLabels;
    headerLabels << "Name" << "Category" << "Current Stock Level" << "Cost Per Gram" << "Show Supplier Orders" << "Starting Balance" << "Total Grams Purchased" << "Total Grams Sold" << "Preferred Supplier" << "Starting Cost";
    ui.tableMHHerbTable->setHorizontalHeaderLabels(headerLabels);

    // ---------- add herb ----------
    connect(ui.btnAHAddHerbToDatabase, &QPushButton::clicked, this, &BodyFixTherapiesSystem::AddHerb);
    connect(ui.btnAHBack, &QPushButton::clicked, this, &BodyFixTherapiesSystem::GoToManageHerbs);

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
                if ((*herbList)[i].showSupplierOrders == 0) {
                    ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem("False"));
                }
                else {
                    ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem("True"));
                }
                break;

            case 5:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem(QString::number((*herbList)[i].startingBalance)));
                break;

            case 6:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem(QString::number((*herbList)[i].totalGramsPurchased)));
                break;

            case 7:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem(QString::number((*herbList)[i].totalGramsSold)));
                break;

            case 8:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].preferredSupplier.c_str()));
                break;

            case 9:
                ui.tableMHHerbTable->setItem(i, j, new QTableWidgetItem((*herbList)[i].startingCost.ToString().c_str()));
                break;
            }
        }
    }
    // make contents of table fill up the entire table space horizontally
    ui.tableMHHerbTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // resize columns to ensure no information is cut off by thin columns
    ui.tableMHHerbTable->resizeColumnsToContents();
}

void BodyFixTherapiesSystem::FilterMHTable()
{
    int filterType = 0;
    if (ui.radioBtnMHDescending->isChecked()) {
        filterType = 1;
    }

    UpdateMHTable(herbHandler.GetFilteredHerbs(ui.comboBoxMHFilterCombo->currentIndex(), filterType));
}

void BodyFixTherapiesSystem::AddHerb()
{
    // check for empty fields
    if (ui.lineEditAHName->text().isEmpty() ||
        ui.lineEditAHCategory->text().isEmpty() ||
        ui.lineEditAHCurrentStockTotal->text().isEmpty() ||
        ui.lineEditAHCostPerGram->text().isEmpty() ||
        ui.lineEditAHStartingBalance->text().isEmpty() ||
        ui.lineEditAHTotalGramsPurchased->text().isEmpty() ||
        ui.lineEditAHTotalGramsSold->text().isEmpty() ||
        ui.lineEditAHPreferredSupplier->text().isEmpty() ||
        ui.lineEditAHStartingCost->text().isEmpty()) {
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
        Herb newHerb(ui.lineEditAHName->text().toStdString(), 
            ui.lineEditAHCategory->text().toStdString(), 
            ui.lineEditAHCurrentStockTotal->text().toInt(), 
            Money(ui.lineEditAHCostPerGram->text().toDouble()), 
            ui.checkBoxAHShowSupplierOrders->isChecked() ? true : false, 
            ui.lineEditAHStartingBalance->text().toInt(), 
            ui.lineEditAHTotalGramsPurchased->text().toInt(), 
            ui.lineEditAHTotalGramsSold->text().toInt(), 
            ui.lineEditAHPreferredSupplier->text().toStdString(), 
            Money(ui.lineEditAHStartingCost->text().toDouble()));

        if (herbHandler.AddHerb(newHerb)) {
            QMessageBox::information(this, "Success", "Herb added to database successfully.");
        }
        else {
            QMessageBox::critical(this, "Error", "Failed to add herb to database.");
        }
    }
}

// navigation buttons
inline void BodyFixTherapiesSystem::GoToMainMenu() { ui.gridStackedWidget->setCurrentIndex(0); }
inline void BodyFixTherapiesSystem::GoToManageHerbs() { 
    ui.gridStackedWidget->setCurrentIndex(1); 
    UpdateMHTable(herbHandler.GetAllHerbs());

}
inline void BodyFixTherapiesSystem::GoToManageFormulas() { ui.gridStackedWidget->setCurrentIndex(2); }
inline void BodyFixTherapiesSystem::GoToManageSuppliers() { ui.gridStackedWidget->setCurrentIndex(3); }
inline void BodyFixTherapiesSystem::GoToSettings() { ui.gridStackedWidget->setCurrentIndex(4); }
inline void BodyFixTherapiesSystem::GoToAddHerb() { ui.gridStackedWidget->setCurrentIndex(5); }
inline void BodyFixTherapiesSystem::QuitApp() { close(); }