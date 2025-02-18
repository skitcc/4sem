/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTableView *tableTransposition;
    QPushButton *transpositionButton;
    QTableView *tableScaling;
    QPushButton *scalingButton;
    QTableView *tableRotate;
    QPushButton *rotationButtton;
    QPushButton *undoButton;
    QWidget *transformationWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(630, 0, 321, 511));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableTransposition = new QTableView(verticalLayoutWidget);
        tableTransposition->setObjectName(QString::fromUtf8("tableTransposition"));

        verticalLayout->addWidget(tableTransposition);

        transpositionButton = new QPushButton(verticalLayoutWidget);
        transpositionButton->setObjectName(QString::fromUtf8("transpositionButton"));

        verticalLayout->addWidget(transpositionButton);

        tableScaling = new QTableView(verticalLayoutWidget);
        tableScaling->setObjectName(QString::fromUtf8("tableScaling"));

        verticalLayout->addWidget(tableScaling);

        scalingButton = new QPushButton(verticalLayoutWidget);
        scalingButton->setObjectName(QString::fromUtf8("scalingButton"));

        verticalLayout->addWidget(scalingButton);

        tableRotate = new QTableView(verticalLayoutWidget);
        tableRotate->setObjectName(QString::fromUtf8("tableRotate"));

        verticalLayout->addWidget(tableRotate);

        rotationButtton = new QPushButton(verticalLayoutWidget);
        rotationButtton->setObjectName(QString::fromUtf8("rotationButtton"));

        verticalLayout->addWidget(rotationButtton);

        undoButton = new QPushButton(verticalLayoutWidget);
        undoButton->setObjectName(QString::fromUtf8("undoButton"));

        verticalLayout->addWidget(undoButton);

        transformationWidget = new QWidget(centralwidget);
        transformationWidget->setObjectName(QString::fromUtf8("transformationWidget"));
        transformationWidget->setGeometry(QRect(0, 0, 600, 600));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 950, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        transpositionButton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\200\320\265\320\275\320\276\321\201", nullptr));
        scalingButton->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\201\321\210\321\202\320\260\320\261\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        rotationButtton->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\262\320\276\321\200\320\276\321\202", nullptr));
        undoButton->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\321\200\320\275\321\203\321\202\321\214\321\201\321\217 \320\275\320\260\320\267\320\260\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
