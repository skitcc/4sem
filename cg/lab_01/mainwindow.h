#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QStandardItemModel>

#include "cartesianWidget.h"
#include "structs.h"

struct Vertexes {
    int A;
    int B;
    int C;

    Vertexes() : A(-1), B(-1), C(-1) {}

    Vertexes(std::initializer_list<int> list) {
        auto it = list.begin();
        if (list.size() >= 1) A = *it++;
        if (list.size() >= 2) B = *it++;
        if (list.size() >= 3) C = *it;
    }
};


struct TriangleWithMedian {
    Triangle triangle;
    Median median;
    double minMedian;
    Vertexes vertexes;

    TriangleWithMedian(): minMedian(std::numeric_limits<double>::max()) {}
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
    QVector<QPointF> points;
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
    void resetScale();

private:
    void editPointFromTable(int row);
    void deletePointFromTable(int row);
};
#endif // MAINWINDOW_H
