#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QStandardItemModel>

#include "cartesianWidget.h"


struct Point {
    double x;
    double y;
};


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QVector<Point> points;
    CartesianAxis *cartesian_axis;
    QStandardItemModel *table;

private slots:
    void onButtonPush(void);
    void onButtonSolve(void);
    void addPointFromClick(double x, double y);
    void deleteAllPoints(void);
    void editPointFromCanvas(double x, double y);
    void deletePointFromCanvas(double x, double y);
    void showTableContextMenu(const QPoint &pos);

private:
    void editPointFromTable(int row);
    void deletePointFromTable(int row);
    double length(double x1, double y1, double x2, double y2);
    double calculate_min_median(struct Point A, struct Point B, struct Point C);

};
#endif // MAINWINDOW_H
