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
    setMinimumSize(600, 600);
    setMaximumSize(600, 600);
};


void CartesianAxis::addPoint(double x, double y) {
    points.append(QPointF(x, y));
    update();
}


void CartesianAxis::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    centerX = width() / 2;
    centerY = height() / 2;

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

void CartesianAxis::updateOffsets() {

    const double minX = std::min({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});
    const double maxX = std::max({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});

    const double minY = std::min({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});
    const double maxY = std::max({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});

    const double mathOriginX = (minX + maxX) / 2.0;
    const double mathOriginY = (minY + maxY) / 2.0;

    offsetX = centerX - (mathOriginX * scale);
    offsetY = centerY + (mathOriginY * scale);

    qDebug() << "offset X " << offsetX;
    qDebug() << "offset Y " << offsetY;
}


void CartesianAxis::resetScale() {
    qDebug() << "resetScale triggered";
    isAutoScaled = false;
    scale = defaultScale;
    update();
}


void CartesianAxis::autoScale() {
    if (triangle_points.size() != 3 || median_points.size() != 2 || isAutoScaled)
        return;

    qDebug() << "autoScale triggered";
    const double minX = std::min({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});
    const double maxX = std::max({triangle_points[0].x(), triangle_points[1].x(), triangle_points[2].x()});

    const double minY = std::min({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});
    const double maxY = std::max({triangle_points[0].y(), triangle_points[1].y(), triangle_points[2].y()});

    const double deltaX = maxX - minX;
    const double deltaY = maxY - minY;

    const int w = this->width() * 0.8;
    const int h = this->height() * 0.8;

    if (deltaX / w < 0.1 || deltaY / h < 0.1) {
        scale = std::min(w * 0.9 / deltaX, h * 0.9 / deltaY);
    } else if (deltaX / w > 0.9 || deltaY / h > 0.9) {
        scale = std::min(w / (deltaX), h / (deltaY));
    } else {
        scale = 1.0;
    }

    updateOffsets();
    isAutoScaled = true;

    qDebug() << "Computed scale: " << scale;
};


QPointF CartesianAxis::convertPixToMath(QPointF point) {
    return QPointF((centerX + point.x()) / scale, (centerY - point.y()) / scale);
}


QPointF CartesianAxis::convertMathToPix(const QPointF& mathPoint) {

    double midX = (triangle_points[0].x() + triangle_points[1].x() + triangle_points[2].x()) / 3.0;
    double midY = (triangle_points[0].y() + triangle_points[1].y() + triangle_points[2].y()) / 3.0;
    return QPointF(centerX + static_cast<int>((mathPoint.x() - midX) * scale)
                  ,centerY - static_cast<int>((mathPoint.y() - midY) * scale));
}


void CartesianAxis::formTriangle(QPainter& painter) {
    QPointF A = convertMathToPix(triangle_points[0]);
    QPointF B = convertMathToPix(triangle_points[1]);
    QPointF C = convertMathToPix(triangle_points[2]);

    painter.drawLine(A, B);
    painter.drawLine(B, C);
    painter.drawLine(A, C);

    painter.setPen(Qt::green);

    painter.drawEllipse(A, 3, 3);
    painter.drawText(A.x() + 5, A.y() + 5, QString::number(1));

    painter.drawEllipse(B, 3, 3);
    painter.drawText(B.x() + 5, B.y() + 5, QString::number(2));

    painter.drawEllipse(C, 3, 3);
    painter.drawText(C.x() + 5, C.y() + 5, QString::number(3));
}


void CartesianAxis::formMedian(QPainter& painter) {
    QPointF startMedian = convertMathToPix(median_points[0]);
    QPointF endMedian = convertMathToPix(median_points[1]);

    painter.drawLine(startMedian, endMedian);
}


void CartesianAxis::formAxis(QPainter& painter) {

    int step = 20;
    int tickLength = 5;

    painter.setPen(Qt::black);
    painter.drawLine(0, centerY, width(), centerY);
    painter.drawLine(centerX, 0, centerX, height());

    painter.drawText(centerX + 5, 15, "Y");
    painter.drawText(width() - 15, centerY - 5, "X");
    painter.drawText(centerX + 5, centerY + 15, "0");

    double scaleFactor = scale / defaultScale;


    for (int i = centerX + step, value = 1; i < width(); i += step, ++value) {
        painter.drawLine(i, centerY - tickLength, i, centerY + tickLength);
        painter.drawText(i - 10, centerY + 20, QString::number(value / scaleFactor, 'f', 0));
    }
    for (int i = centerX - step, value = -1; i > 0; i -= step, --value) {
        painter.drawLine(i, centerY - tickLength, i, centerY + tickLength);
        painter.drawText(i - 10, centerY + 20, QString::number(value / scaleFactor, 'f', 0));
    }
    for (int i = centerY - step, value = 1; i > 0; i -= step, ++value) {

        painter.drawLine(centerX - tickLength, i, centerX + tickLength, i);
        painter.drawText(centerX + 10, i + 5, QString::number(value / scaleFactor, 'f', 0));
    }
    for (int i = centerY + step, value = -1; i < height(); i += step, --value) {
        painter.drawLine(centerX - tickLength, i, centerX + tickLength, i);
        painter.drawText(centerX + 10, i + 5, QString::number(value / scaleFactor, 'f', 0));
    }
}


void CartesianAxis::formDots(QPainter& painter) {
    painter.setPen(Qt::white);
    int countVertex = 1;
    for (const QPointF &point : points) {
        double x = centerX + point.x() * defaultScale;
        double y = centerX - point.y() * defaultScale;
        QPointF p = {x, y};
        painter.drawEllipse(p, 3, 3);
        painter.drawText(p.x() + 5, p.y() + 5, QString::number(countVertex++));
    }
}


void CartesianAxis::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    autoScale();

    qDebug() << "init scale " << scale;

    QPainter painter(this);
    formAxis(painter);
    formDots(painter);
    if (triangle_points.size() == 3) {
        painter.setPen(Qt::red);
        formTriangle(painter);
        painter.setPen(Qt::blue);
        formMedian(painter);
    }

}


void CartesianAxis::deletePointAt(const double x, const double y) {
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
                }
                return;
            }
        }
    }

    emit pointClicked(x, y);
}


void CartesianAxis::deletePoints() {
    points.clear();
    triangle_points.clear();
    median_points.clear();
    resetScale();
    update();
}
