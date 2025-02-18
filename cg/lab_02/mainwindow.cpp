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
    ui->tableTransposition->verticalHeader()->hide();


    for (int column = 0; column < 2; column++) {
        ui->tableTransposition->setColumnWidth(column, TABLEVIEWWIDTH / 2);
        tableTransponse->setItem(0, column, new QStandardItem("0"));
    }
    


    tableScale->setColumnCount(4);
    tableScale->setHorizontalHeaderLabels({"kx", "ky", "cx", "cy"});
    ui->tableScaling->setModel(tableScale);
    ui->tableScaling->verticalHeader()->hide();


    for (int column = 0; column < 4; column++) {
        ui->tableScaling->setColumnWidth(column, TABLEVIEWWIDTH / 4);
        tableScale->setItem(0, column, new QStandardItem("0"));

    }
    


    tableRotate->setColumnCount(3);
    tableRotate->setHorizontalHeaderLabels({"rx", "ry", "angle"});
    ui->tableRotate->setModel(tableRotate);
    ui->tableRotate->verticalHeader()->hide();


    for (int column = 0; column < 3; column++) {
        ui->tableRotate->setColumnWidth(column, TABLEVIEWWIDTH / 3);
        tableRotate->setItem(0, column, new QStandardItem("0"));

    }

    connect(ui->transpositionButton, &QPushButton::clicked, this, &MainWindow::applyTransponse);
    connect(ui->scalingButton, &QPushButton::clicked, this, &MainWindow::applyScale);
    connect(ui->rotationButtton, &QPushButton::clicked, this, &MainWindow::applyRotate);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::applyUndo);

}



void MainWindow::applyTransponse() {
    bool ok1 = false, ok2 = false;

    double dx = 0, dy = 0;

    if (tableTransponse->item(0, 0) && tableTransponse->item(0, 1)) {
        dx = tableTransponse->item(0, 0)->text().toDouble(&ok1);
        dy = tableTransponse->item(0, 1)->text().toDouble(&ok2);
    }


    if (ok1 && ok2)
        transformationWidget->transponse(dx, dy);
    else {
        QMessageBox::information(this, "Ошибка", "Ошибка ввода полей для переноса");
        return;
    }

    update();

}

void MainWindow::applyScale() {
    bool ok1 = false, ok2 = false, ok3 = false, ok4 = false;

    double kx = 0, ky = 0, cx = 0, cy = 0;

    if (tableScale->item(0, 0) && tableScale->item(0, 1) && tableScale->item(0, 2) && tableScale->item(0, 3)) {
        kx = tableScale->item(0, 0)->text().toDouble(&ok1);
        ky = tableScale->item(0, 1)->text().toDouble(&ok2);
        cx = tableScale->item(0, 2)->text().toDouble(&ok3);
        cy = tableScale->item(0, 3)->text().toDouble(&ok4);
    }

    if (ok1 && ok2 && ok3 && ok4) {
        transformationWidget->scale(kx, ky, cx, cy);
    }
    else {
        QMessageBox::information(this, "Ошибка", "Ошибка ввода полей для масштабирования");
        return;
    }


    update();
}

void MainWindow::applyRotate() {
    bool ok1 = false, ok2 = false, ok3 = false;

    double rx = 0, ry = 0, angle = 0;

    if (tableRotate->item(0, 0) && tableRotate->item(0, 1) && tableRotate->item(0, 2)) {
        rx = tableRotate->item(0, 0)->text().toDouble(&ok1);
        ry = tableRotate->item(0, 1)->text().toDouble(&ok2);
        angle = tableRotate->item(0, 2)->text().toDouble(&ok3);
    }

    if (ok1 && ok2 && ok3) {
        transformationWidget->rotate(rx, ry, angle);
    } else {
        QMessageBox::information(this, "Ошибка", "Ошибка ввода полей для поворота");
        return;
    }

    update();
}


void MainWindow::applyUndo() {
    transformationWidget->undo();
}



MainWindow::~MainWindow()
{
    delete ui;
}
