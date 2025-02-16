#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>

#include "transformationWidget.h"


#define TABLEVIEWWIDTH 317


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

private slots:
    void applyTransponse();
    void applyScale();
    void applyRotate();
    void applyUndo();

private:
    QStandardItemModel *tableTransponse;
    QStandardItemModel *tableScale;
    QStandardItemModel *tableRotate;
    Transformation *transformationWidget;

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
