#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // this->resize(1000, 700);

    transformationWidget = new Transformation(ui->transformationWidget);
    transformationWidget->setGeometry(ui->transformationWidget->geometry());

    tableTransponse = new QStandardItemModel(this);
    tableScale = new QStandardItemModel(this);
    tableRotate = new QStandardItemModel(this);


    tableTransponse->setColumnCount(2);
    tableTransponse->setHorizontalHeaderLabels({"dx", "dy"});
    ui->tableTransposition->setModel(tableTransponse);

    for (int column = 0; column < 2; column++) {
        ui->tableTransposition->setColumnWidth(column, TABLEVIEWWIDTH / 2);
    }
    


    tableScale->setColumnCount(4);
    tableScale->setHorizontalHeaderLabels({"kx", "ky", "cx", "cy"});
    ui->scalingButton->setModel(tableScale);

    for (int column = 0; column < 4; column++) {
        ui->scalingButton->setColumnWidth(column, TABLEVIEWWIDTH / 4);
    }
    


    tableRotate->setColumnCount(3);
    tableRotate->setHorizontalHeaderLabels({"rx", "ry", "angle"});
    ui->tableRotate->setModel(tableRotate);

    for (int column = 0; column < 3; column++) {
        ui->tableRotate->setColumnWidth(column, TABLEVIEWWIDTH / 3);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
