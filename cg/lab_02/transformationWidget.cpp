#include "transformationWidget.h"


Transformation::Transformation(QWidget *parent) : QWidget(parent) {
    // setMinimumSize(1200, 1200);
    // setMaximumSize(800, 800);
}



void Transformation::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::green);
    qDebug() << width() << " " << height();
    painter.drawRect(0, 0, width() - 5, height() - 5);

    painter.setPen(Qt::red);
    painter.drawRect(width() / 2, height() / 2, 50, 50);
}



void Transformation::pixToMath(double xPix, double yPix) {
    double xMath = xPix - width() / 2;
    double yMath = height() / 2 - yPix;
}


void Transformation::mathToPix(double xMath, double yMath) {
    double xPix = xMath + width() / 2;
    double yPix = height() / 2 - yMath;
}

void Transformation::transponse() {
    int dx = 20, dy = 20;
    int transeponseMatrix = [[1, 0, 0], [0, 1, 0], [dx, dy, 1]];

}
