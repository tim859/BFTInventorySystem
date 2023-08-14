#include "BodyFixTherapiesSystem.h"
#include <iostream>
#include <qsqldatabase.h>

// constructor
BodyFixTherapiesSystem::BodyFixTherapiesSystem(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    // connect btnConnectToDB to ConnectToDB() function
    connect(ui.btnConnectToDB, &QPushButton::clicked, this, &BodyFixTherapiesSystem::ConnectToDB);
}

// destructor
BodyFixTherapiesSystem::~BodyFixTherapiesSystem()
{}

void BodyFixTherapiesSystem::ConnectToDB() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/Misc Coding Projects/C++ Projects/BodyFixTherapies/BodyFixTherapiesSystem/BFTDB.db");

    if (db.open()) {
        UpdateLabel("yes");
    }
    else {
        UpdateLabel("no");
    }
}

void BodyFixTherapiesSystem::UpdateLabel(string text) {
    ui.lblConnectDB->setText(QString::fromStdString(text));
}