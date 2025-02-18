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


bool CartesianAxis::checkIsInTringle(QPointF point, QPointF vertex) {

    if (std::abs(point.x() - vertex.x()) < EPS && std::abs(point.y() - vertex.y()) < EPS)
        return true;
    return false;

}

void CartesianAxis::deleteNotTriangle() {
    for (int i = points.size() - 1; i >= 0; i--) {
        bool isInTriangle = false;


        isInTriangle = checkIsInTringle(points[i], triangle.A) 
                    || checkIsInTringle(points[i], triangle.B)
                    || checkIsInTringle(points[i],  triangle.C);

        if (!isInTriangle) {
            points.removeAt(i);
        };

        }

    update();
}

void CartesianAxis::updateOffsets() {


    const double minX = std::min({triangle.A.x(), triangle.B.x(), triangle.C.x()});
    const double maxX = std::max({triangle.A.x(), triangle.B.x(), triangle.C.x()});

    const double minY = std::min({triangle.A.y(), triangle.B.y(), triangle.C.y()});
    const double maxY = std::max({triangle.A.y(), triangle.B.y(), triangle.C.y()});

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
    if (!triangle.isValid || !median.isValid || isAutoScaled)
        return;

    qDebug() << "autoScale triggered";
    const double minX = std::min({triangle.A.x(), triangle.B.x(), triangle.C.x()});
    const double maxX = std::max({triangle.A.x(), triangle.B.x(), triangle.C.x()});

    const double minY = std::min({triangle.A.y(), triangle.B.y(), triangle.C.y()});
    const double maxY = std::max({triangle.A.y(), triangle.B.y(), triangle.C.y()});

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

    double midX = (triangle.A.x() + triangle.B.x() + triangle.C.x()) / 3.0;
    double midY = (triangle.A.y() + triangle.B.y() + triangle.C.y()) / 3.0;
    return QPointF(centerX + static_cast<int>((mathPoint.x() - midX) * scale)
                  ,centerY - static_cast<int>((mathPoint.y() - midY) * scale));
}



void CartesianAxis::formTriangle(QPainter& painter) {
    QPointF A = convertMathToPix(triangle.A);
    QPointF B = convertMathToPix(triangle.B);
    QPointF C = convertMathToPix(triangle.C);

    painter.setPen(Qt::red);
    painter.drawLine(A, B);
    painter.drawLine(B, C);
    painter.drawLine(A, C);

    painter.setPen(Qt::green);

    int widgetWidth = width();
    int widgetHeight = height();
    int textOffsetX, textOffsetY;

    auto adjustTextPosition = [&](QPointF point) -> QPointF {
        textOffsetX = 5;
        textOffsetY = -5;

        if (point.x() > widgetWidth - 50) textOffsetX = -50;
        if (point.y() < 30) textOffsetY = 15;
        if (point.x() < 30) textOffsetX = 10;
        if (point.y() > widgetHeight - 30) textOffsetY = -15;

        return QPointF(point.x() + textOffsetX, point.y() + textOffsetY);
    };

    painter.drawEllipse(A, 3, 3);
    painter.drawText(adjustTextPosition(A), QString("%1 (%2, %3)").arg(triangle.vertexes.numA).arg(triangle.A.x(), 0, 'f', 2).arg(triangle.A.y(), 0, 'f', 2));

    painter.drawEllipse(B, 3, 3);
    painter.drawText(adjustTextPosition(B), QString("%1 (%2, %3)").arg(triangle.vertexes.numB).arg(triangle.B.x(), 0, 'f', 2).arg(triangle.B.y(), 0, 'f', 2));

    painter.drawEllipse(C, 3, 3);
    painter.drawText(adjustTextPosition(C), QString("%1 (%2, %3)").arg(triangle.vertexes.numC).arg(triangle.C.x(), 0, 'f', 2).arg(triangle.C.y(), 0, 'f', 2));
}



void CartesianAxis::formMedian(QPainter& painter) {
    QPointF startMedian = convertMathToPix(median.startMedian);
    QPointF endMedian = convertMathToPix(median.endMedian);

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
}


void CartesianAxis::formDot(QPainter& painter, QPointF point, int numVertex) {
        double x = centerX + point.x() * defaultScale;
        double y = centerY - point.y() * defaultScale;
        QPointF p = {x, y};
        painter.drawEllipse(p, 3, 3);
        painter.drawText(p.x() + 5, p.y() + 5, QString::number(numVertex));
}



void CartesianAxis::formDots(QPainter& painter) {
    painter.setPen(Qt::green);
    painter.setBrush(Qt::green);
    int countVertex = 1;

    if (triangle.isValid) {
        formDot(painter, triangle.A, triangle.vertexes.numA);
        formDot(painter, triangle.B, triangle.vertexes.numB);
        formDot(painter, triangle.C, triangle.vertexes.numC);
    }
    else {
        for (const QPointF &point : points) {
            double x = centerX + point.x() * defaultScale;
            double y = centerY - point.y() * defaultScale;
            QPointF p = {x, y};
            painter.drawEllipse(p, 3, 3);
            painter.drawText(p.x() + 5, p.y() + 5, QString::number(countVertex++));
        }
    }
}

void CartesianAxis::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    qDebug() << "init scale " << scale;

    QPainter painter(this);
    formAxis(painter);
    formDots(painter);
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::darkGray);
    setAutoFillBackground(true);
    setPalette(Pal);

    autoScale();

    if (triangle.isValid) {
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

void CartesianAxis::setTriangle(Triangle import_triangle) {
    triangle = import_triangle;
    update();
}

void CartesianAxis::setMedian(Median import_median) {
    median = import_median;
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
    triangle.reset();
    median.reset();
    resetScale();
    update();
}
