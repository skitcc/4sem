#include "cartesianWidget.h"

#include <QPainter>
#include <QFont>
#include <iostream>
#include <QMenu>
#include <QDebug>


CartesianAxis::CartesianAxis(QWidget *parent) 
    : QWidget(parent),
      scale(20.0),
      offsetX(width() / 2),
      offsetY(height() / 2) {
    setMinimumSize(400, 400);
};


void CartesianAxis::addPoint(double x, double y) {
    points.append(QPointF(x, y));
    update();
}

void CartesianAxis::deleteNotTriangle() {
    for (int i = points.size() - 1; i >= 0; i--) {
        bool isInTriangle = false;

        for (int j = 0; j < triangle_points.size(); j++) {
            if (std::abs(points[i].x() - triangle_points[j].x()) < EPS &&  
                std::abs(points[i].y() - triangle_points[j].y()) < EPS) {
                isInTriangle = true;
                break;
            }
        }

        if (!isInTriangle) {
            points.removeAt(i);
        }
    }
    update();
}

void CartesianAxis::autoScale() {
    if (triangle_points.size() != 3 || median_points.size() != 2)
        return;

    const double minX = std::min({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});
    const double maxX = std::max({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});

    const double minY = std::min({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});
    const double maxY = std::max({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});

    const double deltaX = maxX - minX;
    const double deltaY = maxY - minY;

    const int w = this->width();
    const int h = this->height();

    if (deltaX / w < 0.1 || deltaY / h < 0.1) {
        scale = std::min(w * 0.9 / deltaX, h * 0.9 / deltaY);
    } else if (deltaX / w > 0.9 || deltaY / h > 0.9) {
        scale = std::min(w / (deltaX * 3), h / (deltaY * 3));
    } else {
        scale = 1.0;
    }

    qDebug() << "Computed scale: " << scale;
}




void CartesianAxis::paintEvent(QPaintEvent *event) {

    Q_UNUSED(event);

    autoScale();
    double centerX = width() / 2;
    double centerY = height() / 2;

    QPainter painter(this);


    qDebug() << "width " << width();
    qDebug() << "height " << height();

    qDebug() << "scale" << scale;

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

    QBrush brush(Qt::green);
    painter.setBrush(brush);

    int vertexCount = 1;

    for (const QPointF &point : points) {
        int px = centerX + static_cast<int>(point.x() * scale);
        int py = centerY - static_cast<int>(point.y() * scale);
        painter.drawEllipse(QPointF(px, py), 3, 3);
        painter.drawText(QPointF(px + 5, py), QString::number(vertexCount++));
    }

    if (triangle_points.size() == 3) {
        painter.setPen(Qt::red);

        double midX = (triangle_points[0].x() + triangle_points[1].x() + triangle_points[2].x()) / 3.0;
        double midY = (triangle_points[0].y() + triangle_points[1].y() + triangle_points[2].y()) / 3.0;

        int x1 = centerX + static_cast<int>((triangle_points[0].x() - midX) * scale);
        int y1 = centerY - static_cast<int>((triangle_points[0].y() - midY) * scale);

        int x2 = centerX + static_cast<int>((triangle_points[1].x() - midX) * scale);
        int y2 = centerY - static_cast<int>((triangle_points[1].y() - midY) * scale);

        int x3 = centerX + static_cast<int>((triangle_points[2].x() - midX) * scale);
        int y3 = centerY - static_cast<int>((triangle_points[2].y() - midY) * scale);

        painter.drawLine(x1, y1, x2, y2);
        painter.drawLine(x2, y2, x3, y3);
        painter.drawLine(x3, y3, x1, y1);

        int xStartMedian = centerX + static_cast<int>((median_points[0].x() - midX) * scale);
        int yStartMedian = centerY - static_cast<int>((median_points[0].y() - midY) * scale);

        int xEndMedian = centerX + static_cast<int>((median_points[1].x() - midX) * scale);
        int yEndMedian = centerY - static_cast<int>((median_points[1].y() - midY) * scale);

        painter.drawLine(xStartMedian, yStartMedian, xEndMedian, yEndMedian);

        painter.setPen(Qt::blue);

        painter.drawLine(xStartMedian, yStartMedian, xEndMedian, yEndMedian);
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



void CartesianAxis::setTriangle(const QPointF& a, const QPointF& b, const QPointF& c) {
    triangle_points = {a, b, c};
    update();
}

void CartesianAxis::setMedian(const QPointF& a, const QPointF& b) {
    median_points = {a, b};
    update();
}


void CartesianAxis::mousePressEvent(QMouseEvent *event) {

    double centerX = width() / 2;
    double centerY = height() / 2;

    double local_scale = 20.0;

    double x = (event->x() - centerX) / local_scale;
    double y = (centerY - event->y()) / local_scale;


    double xClicked = (event->x() - centerX) / local_scale;
    double yClicked = (centerY - event->y()) / local_scale;

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
                }
                return;
            }
        }
    }



    emit pointClicked(xClicked, yClicked);
}


void CartesianAxis::deletePoints() {
    points.clear();
    triangle_points.clear();
    median_points.clear();
    scale = 20.0;
    update();
}


