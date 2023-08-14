#pragma once

#include <QtWidgets/QWidget>
#include "ui_BodyFixTherapiesSystem.h"

using namespace std;

class BodyFixTherapiesSystem : public QWidget
{
    Q_OBJECT

public:
    BodyFixTherapiesSystem(QWidget *parent = nullptr);
    ~BodyFixTherapiesSystem();

private slots:
    void ConnectToDB();

private:
    Ui::BodyFixTherapiesSystemClass ui;
    void UpdateLabel(string text);
};
