#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>
#include <iostream>
#include <cmath>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cartesian_axis = new CartesianAxis(ui->cartesianWidget);
    cartesian_axis->setGeometry(ui->cartesianWidget->geometry());


    table = new QStandardItemModel(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"X", "Y"});

    ui->tableView->setModel(table);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);


    ui->lineEdit_1->setPlaceholderText("Введите x");
    ui->lineEdit_2->setPlaceholderText("Введите y");

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonPush);
    connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::onButtonSolve);
    connect(cartesian_axis, &CartesianAxis::pointClicked, this, &MainWindow::addPointFromClick);
    connect(ui->pushButtonDeleteAll, &QPushButton::clicked, this, &MainWindow::deleteAllPoints);
    connect(cartesian_axis, &CartesianAxis::pointToEdit, this, &MainWindow::editPointFromCanvas);
    connect(cartesian_axis, &CartesianAxis::pointToDelete, this, &MainWindow::deletePointFromCanvas);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &MainWindow::showTableContextMenu);
}

void MainWindow::onButtonPush() {

    QPainter painter(this);

    QString xText = ui->lineEdit_1->text();
    QString yText = ui->lineEdit_2->text();


    bool okX, okY;

    double x = xText.toDouble(&okX);
    double y = yText.toDouble(&okY);
    
    if (!okX || !okY) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения для x и y!");
        return;
    };

    points.append({x, y});

    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QStandardItem(QString::number(x)));        
    table->setItem(row, 1, new QStandardItem(QString::number(y)));  

    cartesian_axis->addPoint(x, y);  

    QMessageBox::information(this, "Успех", QString("Точка (%1, %2) добавлена!").arg(x).arg(y));
}


void MainWindow::addPointFromClick(double x, double y) {
    cartesian_axis->addPoint(x, y);
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QStandardItem(QString::number(x)));        
    table->setItem(row, 1, new QStandardItem(QString::number(y)));  
}


void MainWindow::deleteAllPoints(void) {
    cartesian_axis->deletePoints();
    table->clear();
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"X", "Y"});
    points.clear();
    ui->lineEdit_1->clear();
    ui->lineEdit_2->clear();
}


void MainWindow::editPointFromCanvas(double x, double y) {
    bool okX, okY;

    double newX = QInputDialog::getDouble(this, "Изменить координату X","Введите новое значение координаты X:", x, -1000, 1000, 2, &okX);
    double newY = QInputDialog::getDouble(this, "Изменить координату Y","Введите новое значение координаты Y:", y, -1000, 1000, 2, &okY);


    if (okX && okY) {
        cartesian_axis->deletePointAt(x, y);
        cartesian_axis->addPoint(newX, newY);

        for (int i = 0; i < table->rowCount(); i++) {
            double tableX = table->item(i, 0)->text().toDouble();
            double tableY = table->item(i, 1)->text().toDouble();
            if (std::abs(tableX - x) < 1e-6 && std::abs(tableY - y) < 1e-6) {
                table->item(i, 0)->setText(QString::number(newX));
                table->item(i, 1)->setText(QString::number(newY));
                break;
            }
        }
    }


    for (int i = 0; i < points.size(); i++) {
        if (std::abs(x - points[i].x) <= EPS && std::abs(y - points[i].y) <= EPS) {
            points.removeAt(i);
            update();
            break;
        }
    }
}

void MainWindow::deletePointFromCanvas(double x, double y) {
    int row = -1;

    for (int i = 0; i < table->rowCount(); ++i) {
        double tableX = table->item(i, 0)->text().toDouble();
        double tableY = table->item(i, 1)->text().toDouble();

        if (std::abs(tableX - x) <= EPS && std::abs(tableY - y) <= EPS) {
            row = i;
            break;
        }
    }

    if (row != -1) {
        table->removeRow(row);
        for (int i = 0; i < points.size(); ++i) {
            if (std::abs(points[i].x - x) <= EPS && std::abs(points[i].y - y) <= EPS) {
                points.removeAt(i);
                break;
            }
        }

        cartesian_axis->deletePointAt(x, y);
    }
}


void MainWindow::showTableContextMenu(const QPoint &pos) {
    QModelIndex index = ui->tableView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    QAction *editAction = menu.addAction("Изменить координаты");
    QAction *deleteAction = menu.addAction("Удалить точку");

    QAction *selectedAction = menu.exec(ui->tableView->viewport()->mapToGlobal(pos));
    if (selectedAction == editAction) {
        editPointFromTable(index.row());
    } else if (selectedAction == deleteAction) {
        deletePointFromTable(index.row());
    }
}


void MainWindow::editPointFromTable(int row) {
    double x = table->item(row, 0)->text().toDouble();
    double y = table->item(row, 1)->text().toDouble();

    bool okX, okY;
    double newX = QInputDialog::getDouble(this, "Изменить координату X",
                                          "Введите новое значение X:", x, -1000, 1000, 2, &okX);
    double newY = QInputDialog::getDouble(this, "Изменить координату Y",
                                          "Введите новое значение Y:", y, -1000, 1000, 2, &okY);

    if (okX && okY) {
        table->item(row, 0)->setText(QString::number(newX));
        table->item(row, 1)->setText(QString::number(newY));
        cartesian_axis->deletePointAt(x, y);
        cartesian_axis->addPoint(newX, newY);
    }
}



void MainWindow::deletePointFromTable(int row) {
    double x = table->item(row, 0)->text().toDouble();
    double y = table->item(row, 1)->text().toDouble();
    table->removeRow(row);
    cartesian_axis->deletePointAt(x, y);
}


double MainWindow::length(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


double MainWindow::calculate_min_median(struct Point A, struct Point B, struct Point C) {
    double ma, mb, mc;
    double ab, bc, ac; 

    ab = length(A.x, A.y, B.x, B.y);
    bc = length(B.x, B.y, C.x, C.y);
    ac = length(A.x, A.y, C.x, C.y);


    ma = 0.5 * sqrt(2 * pow(ab, 2) + 2 * pow(ac, 2) - pow(bc, 2));
    mb = 0.5 * sqrt(2 * pow(ab, 2) + 2 * pow(bc, 2) - pow(ac, 2));
    mc = 0.5 * sqrt(2 * pow(bc, 2) + 2 * pow(ac, 2) - pow(ab, 2));

    return std::min({ma, mb, mc});


}

void MainWindow::onButtonSolve() {

    QVector<Point> real_points;

    for (int i = 0; i < table->rowCount(); ++i) {
        double tableX = table->item(i, 0)->text().toDouble();
        double tableY = table->item(i, 1)->text().toDouble();
        real_points.append({tableX, tableY});
    }

    double minMedian = 1e308;

    if (real_points.size() < 3) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно точек для построения треугольника!");
        return;
    }

    Point minA, minB, minC;
    for (int i = 0; i < real_points.size(); i++) {
        for (int j = i + 1; j < real_points.size(); j++) {
            for (int k = j + 1; k < real_points.size(); k++) {
                double median = calculate_min_median(real_points[i], real_points[j], real_points[k]);
                if (median < minMedian) {
                    minMedian = median;
                    minA = real_points[i];
                    minB = real_points[j];
                    minC = real_points[k];
                }
            }
        }
    }



    // Выводим результат
    QMessageBox::information(this, "Результат", QString("Треугольник с минимальной медианой: \n"
                                                        "A(%1, %2)\nB(%3, %4)\nC(%5, %6)\n"
                                                        "Минимальная медиана: %7")
                             .arg(minA.x).arg(minA.y)
                             .arg(minB.x).arg(minB.y)
                             .arg(minC.x).arg(minC.y)
                             .arg(minMedian));
}




MainWindow::~MainWindow()
{
    delete ui;
}


