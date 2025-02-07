#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>


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
            qDebug() << tableX;
            qDebug() << tableY;
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

void MainWindow::onButtonSolve() {
    return;
}


MainWindow::~MainWindow()
{
    delete ui;
}


