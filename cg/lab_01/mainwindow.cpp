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

    this->resize(1200, 700);

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
    connect(ui->pushButtonResetScale, &QPushButton::clicked, this, &MainWindow::resetScale);
}

void MainWindow::onButtonPush() {
    QString xText = ui->lineEdit_1->text();
    QString yText = ui->lineEdit_2->text();


    bool okX, okY;

    double x = xText.toDouble(&okX);
    double y = yText.toDouble(&okY);
    
    if (!okX || !okY) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения для x и y!");
        return;
    };

    points.append(QPointF(x, y));

    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QStandardItem(QString::number(x)));        
    table->setItem(row, 1, new QStandardItem(QString::number(y)));  

    cartesian_axis->addPoint(x, y);  

    QMessageBox::information(this, "Успех", QString("Точка (%1, %2) добавлена!").arg(x).arg(y));
}

void MainWindow::resetScale() {
    cartesian_axis->resetScale();
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
        if (std::abs(x - points[i].x()) <= EPS && std::abs(y - points[i].y()) <= EPS) {
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
            if (std::abs(points[i].x() - x) <= EPS && std::abs(points[i].y() - y) <= EPS) {
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


double length(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


double calculate_min_median(struct Triangle& triangle, struct Median& median) {
    double ma, mb, mc;
    double ab, bc, ac; 

    ab = length(triangle.A.x(), triangle.A.y(), triangle.B.x(), triangle.B.y());
    bc = length(triangle.B.x(), triangle.B.y(), triangle.C.x(), triangle.C.y());
    ac = length(triangle.A.x(), triangle.A.y(), triangle.C.x(), triangle.C.y());


    ma = 0.5 * sqrt(2 * pow(ab, 2) + 2 * pow(ac, 2) - pow(bc, 2));
    mb = 0.5 * sqrt(2 * pow(ab, 2) + 2 * pow(bc, 2) - pow(ac, 2));
    mc = 0.5 * sqrt(2 * pow(bc, 2) + 2 * pow(ac, 2) - pow(ab, 2));

    double minMedian = std::min({ma, mb, mc});


    if (minMedian == ma) {
        median.startMedian = triangle.A;
        median.endMedian = {(triangle.B.x() + triangle.C.x()) / 2, (triangle.B.y() + triangle.C.y()) / 2};
    }
    else if (minMedian == mb) {
        median.startMedian = triangle.B;
        median.endMedian = {(triangle.A.x() + triangle.C.x()) / 2, (triangle.A.y() + triangle.C.y()) / 2};
    }
    else {
        median.startMedian = triangle.C;
        median.endMedian = {(triangle.A.x() + triangle.B.x()) / 2, (triangle.A.y() + triangle.B.y()) / 2};
    }

    return minMedian;

}


bool isDegenerateTriangle(const Triangle& triangle) {
    return (std::abs(triangle.A.x() - triangle.B.x()) <= EPS && std::abs(triangle.B.x() - triangle.C.x()) <= EPS) ||
           (std::abs(triangle.A.y() - triangle.B.y()) <= EPS && std::abs(triangle.B.y() - triangle.C.y()) <= EPS);
}



TriangleWithMedian findTriangleWithMinMedian(const QVector<QPointF>& points, int& countTriangles, int& countDegenerates) {

    TriangleWithMedian bestTriangle;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            for (int k = j + 1; k < points.size(); k++) {
                Triangle triangle = {points[i], points[j], points[k]};
                Median medianVertexes;
                countTriangles++;
                if (isDegenerateTriangle(triangle)) {
                    countDegenerates++;
                    continue;
                }

                double median = calculate_min_median(triangle, medianVertexes);
                if (median < bestTriangle.minMedian) {
                    bestTriangle.minMedian = median;
                    bestTriangle.triangle = {triangle.A, triangle.B, triangle.C};
                    bestTriangle.median = {medianVertexes.startMedian, medianVertexes.endMedian};
                    bestTriangle.vertexes = {i + 1, j + 1, k + 1};
                }
            }
        }
    }

    return bestTriangle;
}




void MainWindow::onButtonSolve() {

    QVector<QPointF> real_points;

    for (int i = 0; i < table->rowCount(); ++i) {
        double tableX = table->item(i, 0)->text().toDouble();
        double tableY = table->item(i, 1)->text().toDouble();
        real_points.append(QPointF{tableX, tableY});
    }

    double minMedian = std::numeric_limits<double>::max();

    if (real_points.size() < 3) {
        QMessageBox::warning(this, "Ошибка", "Недостаточно точек для построения треугольника!");
        return;
    }

    int countDegenerates = 0;
    int countTriangles = 0;


    TriangleWithMedian bestTriangle = findTriangleWithMinMedian(real_points, countTriangles, countDegenerates);

    if (countDegenerates == countTriangles) {
        QMessageBox::information(this, "Результат", "Все введенные треугольники являются вырожденными!");
        return;
    }
    for (int i = real_points.size() - 1; i >= 0; i--) {
        if (i != bestTriangle.vertexes.A - 1 && i != bestTriangle.vertexes.B - 1 && i != bestTriangle.vertexes.C - 1) {
            real_points.removeAt(i);
            deletePointFromTable(i);
        }
    }
    cartesian_axis->setTriangle(bestTriangle.triangle);
    cartesian_axis->setMedian(bestTriangle.median);
    cartesian_axis->deleteNotTriangle();
    cartesian_axis->resetScale();

    QMessageBox::information(this, "Результат", QString("Не вырожденный треугольник с минимальной медианой: \n"
                                                        "%1(%2, %3)\n%4(%5, %6)\n%7(%8, %9)\n"
                                                        "Минимальная медиана: %10\n"
                                                        "Количество вырожденных треугольников: %11\n")
                             .arg(bestTriangle.vertexes.A)
                             .arg(bestTriangle.triangle.A.x()).arg(bestTriangle.triangle.A.y())
                             .arg(bestTriangle.vertexes.B)
                             .arg(bestTriangle.triangle.B.x()).arg(bestTriangle.triangle.B.y())
                             .arg(bestTriangle.vertexes.C)
                             .arg(bestTriangle.triangle.C.y()).arg(bestTriangle.triangle.C.y())
                             .arg(bestTriangle.minMedian)
                             .arg(countDegenerates));




}




MainWindow::~MainWindow()
{
    delete ui;
}


