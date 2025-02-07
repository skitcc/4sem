#include "cartesianWidget.h"

#include <QPainter>
#include <QFont>
#include <iostream>
#include <QMenu>


CartesianAxis::CartesianAxis(QWidget *parent) : QWidget(parent) {
    setMinimumSize(200, 200);
};


void CartesianAxis::addPoint(double x, double y) {
    points.append(QPointF(x, y));
    update();
}


void CartesianAxis::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    int centerX = width() / 2;
    int centerY = height() / 2;

    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.drawLine(0, centerY, width(), centerY);
    painter.drawLine(centerX, 0, centerX, height());


    painter.drawText(centerX + 5, 15, "Y");
    painter.drawText(width() - 15, centerY - 5, "X");
    painter.drawText(centerX + 5, centerY + 15, "0");


    int tickLength = 5;
    int step = 20;


    for (int i = centerX + step, value = 1; i < width(); i += step, ++value) {
        painter.drawLine(i, centerY - tickLength, i, centerY + tickLength);
        painter.drawText(i - 10, centerY + 20, QString::number(value));
    }
    for (int i = centerX - step, value = -1; i > 0; i -= step, --value) {
        painter.drawLine(i, centerY - tickLength, i, centerY + tickLength);
        painter.drawText(i - 10, centerY + 20, QString::number(value));
    }

    for (int i = centerY - step, value = 1; i > 0; i -= step, ++value) {
        painter.drawLine(centerX - tickLength, i, centerX + tickLength, i);
        painter.drawText(centerX + 10, i + 5, QString::number(value));
    }
    for (int i = centerY + step, value = -1; i < height(); i += step, --value) {
        painter.drawLine(centerX - tickLength, i, centerX + tickLength, i);
        painter.drawText(centerX + 10, i + 5, QString::number(value));
    }


    double scale = 20.0;

    painter.setPen(Qt::green);
    QBrush brush(Qt::green);
    painter.setBrush(brush);



    for (const QPointF &point : points) {
        int px = centerX + static_cast<int>(point.x() * scale);
        int py = centerY - static_cast<int>(point.y() * scale);
        painter.drawEllipse(QPointF(px, py), 3, 3);
    }


};


void CartesianAxis::deletePointAt(double x, double y) {
    for (int i = 0; i < points.size(); i++) {
        if (std::abs(x - points[i].x()) <= EPS && std::abs(y - points[i].y()) <= EPS) {
            points.removeAt(i);
            update();
            break;
        }
    }
}


void CartesianAxis::mousePressEvent(QMouseEvent *event) {


    double scale = 20.0;

    double centerX = width() / 2;
    double centerY = height() / 2;

    double x = (event->x() - centerX) / scale;
    double y = (centerY - event->y()) / scale;

    double pointRadius = 5.0;

    if (event->button() == Qt::RightButton) {
        for (const QPointF &point : points) {
            double dx = std::abs(point.x() - x);
            double dy = std::abs(point.y() - y);

            if (dx * scale <= pointRadius && dy * scale <= pointRadius) {
                QMenu menu(this);
                QAction *editAction = menu.addAction("Изменить координаты");
                QAction *removeAction = menu.addAction("Удалить точку");

                QAction *selectedAction = menu.exec(event->globalPos());
                if (selectedAction == editAction) {
                    emit pointToEdit(point.x(), point.y());
                } else if (selectedAction == removeAction) {
                    emit pointToDelete(point.x(), point.y());
                    // deletePointAt(point.x(), point.y());

                }
                return;
            }
        }
    }



    emit pointClicked(x, y);
}


void CartesianAxis::deletePoints() {
    points.clear();
    update();
}


