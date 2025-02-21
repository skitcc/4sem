#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    tableTransponse = new QStandardItemModel(this);
    tableScale = new QStandardItemModel(this);
    tableRotate = new QStandardItemModel(this);

    tableTransponse->setColumnCount(3);
    tableTransponse->setHorizontalHeaderLabels({"dx", "dy", "dz"});
    ui->tableTransposition->setModel(tableTransponse);
    ui->tableTransposition->verticalHeader()->hide();

    for (int column = 0; column < 3; column++) {
        ui->tableTransposition->setColumnWidth(column, TABLEVIEWWIDTH / 3);
        tableTransponse->setItem(0, column, new QStandardItem("0"));
    }

    tableScale->setColumnCount(4);
    tableScale->setHorizontalHeaderLabels({"kx", "ky", "kz"});
    ui->tableScaling->setModel(tableScale);
    ui->tableScaling->verticalHeader()->hide();

    for (int column = 0; column < 3; column++) {
        ui->tableScaling->setColumnWidth(column, TABLEVIEWWIDTH / 3);
        tableScale->setItem(0, column, new QStandardItem("0"));
    }

    tableRotate->setColumnCount(3);
    tableRotate->setHorizontalHeaderLabels({"angle x", "angle y", "angle z"});
    ui->tableRotate->setModel(tableRotate);
    ui->tableRotate->verticalHeader()->hide();

    for (int column = 0; column < 3; column++) {
        ui->tableRotate->setColumnWidth(column, TABLEVIEWWIDTH / 3);
        tableRotate->setItem(0, column, new QStandardItem("0"));
    }

    scene = new QGraphicsScene();
    ui->planeWidget->setScene(scene);
    scene->setBackgroundBrush(Qt::white);

    set_figure(figure, "../data/pyramid.txt");

    connect(ui->transpositionButton, &QPushButton::clicked, this, &MainWindow::applyTransponse);
    connect(ui->scalingButton, &QPushButton::clicked, this, &MainWindow::applyScale);
    connect(ui->rotationButtton, &QPushButton::clicked, this, &MainWindow::applyRotate);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    draw_figure(figure, view);
}

void MainWindow::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    *view = {.height = height(), .width = width(), .scene = scene};
}

void MainWindow::applyTransponse() {
    bool ok1 = false, ok2 = false, ok3 = false;

    double dx = 0, dy = 0, dz = 0;

    if (tableTransponse->item(0, 0) && tableTransponse->item(0, 1) && tableTransponse->item(0, 2)) {
        dx = tableTransponse->item(0, 0)->text().toDouble(&ok1);
        dy = tableTransponse->item(0, 1)->text().toDouble(&ok2);
        dz = tableTransponse->item(0, 2)->text().toDouble(&ok3);
    }

    if (ok1 && ok2 && ok3) { 
        transponse_t transponse = {dx, dy, dz};
        connection_t connection = {view, TRANSPONSE, .transponse_params = transponse};
        handle_action(connection, figure);
    }

    update();
}

void MainWindow::applyScale() {
    bool ok1 = false, ok2 = false, ok3 = false;

    double kx = 0, ky = 0, kz = 0;

    if (tableScale->item(0, 0) && tableScale->item(0, 1) && tableScale->item(0, 2)) {
        kx = tableScale->item(0, 0)->text().toDouble(&ok1);
        ky = tableScale->item(0, 1)->text().toDouble(&ok2);
        kz = tableScale->item(0, 2)->text().toDouble(&ok3);
    }

    if (ok1 && ok2 && ok3) {
        scale_t scale = {kx, ky, kz};
        connection_t connection = {view, SCALE, .scale_params = scale};
        handle_action(connection, figure);
    } else {
        QMessageBox::information(this, "Ошибка", "Ошибка ввода полей для масштабирования");
        return;
    }

    update();
}

void MainWindow::applyRotate() {
    bool ok1 = false, ok2 = false, ok3 = false;

    double angleX = 0, angleY = 0, angleZ = 0;

    if (tableRotate->item(0, 0) && tableRotate->item(0, 1) && tableRotate->item(0, 2)) {
        angleX = tableRotate->item(0, 0)->text().toDouble(&ok1);
        angleY = tableRotate->item(0, 1)->text().toDouble(&ok2);
        angleZ = tableRotate->item(0, 2)->text().toDouble(&ok3);
    }

    if (ok1 && ok2 && ok3) {
        rotate_t rotate = {angleX, angleY, angleZ};
        connection_t connection = {view, ROTATE, .rotate_params = rotate};
        handle_action(connection, figure);
    } else {
        QMessageBox::information(this, "Ошибка", "Ошибка ввода полей для поворота");
        return;
    }

    update();
}

MainWindow::~MainWindow() {
    delete tableRotate;
    delete tableScale;
    delete tableTransponse;
    delete scene;
    delete ui;
}
