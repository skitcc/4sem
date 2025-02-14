#ifndef TRANSFORMATION_WIDGET_H
#define TRANSFORMATION_WIDGET_H


#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>



class Transformation : public QWidget {
    Q_OBJECT

public:
    Transformation(QWidget *parent = nullptr);
    void transponse();
    void mathToPix();
    void pixToMath(double xPix, double yPix);
    ~Transformation() override {};

protected:
    void paintEvent(QPaintEvent *event) override;
};



#endif