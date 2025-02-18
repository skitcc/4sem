#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMessageBox>


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


private:
    QStandardItemModel *tableScale;
    QStandardItemModel *tableRotate;
    QStandardItemModel *tableTransponse;


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
