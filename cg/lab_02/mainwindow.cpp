#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1200, 700);

    tableTransponse = new QStandartItemModel(this);
    tableScale = new QStandartItemModel(this);
    tableRotate = new QStandartItemModel(this);

    ui->tableTransposition->setModel(tableTransponse);
    ui->scalingButton->setModel(tableScale);
    ui->tableRotate->setModel(tableRotate);

    


    

    
    


}

MainWindow::~MainWindow()
{
    delete ui;
}
