#include "cartesianWidget.h"

#include <QPainter>
#include <QFont>
#include <iostream>
#include <QMenu>
#include <QDebug>


TriangleManager::TriangleManager(QObject *parent) : QObject(parent) {}

CartesianGrid::CartesianGrid(QObject *parent) : QObject(parent) {}


CartesianAxis::CartesianAxis(QWidget *parent)
    : QWidget(parent), scale(20.0) {
    
    setMinimumSize(600, 600);
    setMaximumSize(600, 600);


    triangleManager = new TriangleManager(this);
    cartesianGrid = new CartesianGrid(this);

    cartesianGrid->setParent(this);

    // Исправляем connect() (нельзя передавать QOverload<>)
    bool success = connect(cartesianGrid, &CartesianGrid::pointsUpdated, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    qDebug() << "connect(pointsUpdated) success:" << success;
    success = connect(cartesianGrid, &CartesianGrid::pointDeleted, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    qDebug() << "connect(pointDeleted) success:" << success;
    success = connect(triangleManager, &TriangleManager::triangleSetted, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    qDebug() << "connect(triangleSetted) success:" << success;
    success = connect(triangleManager, &TriangleManager::medianSetted, this, static_cast<void (QWidget::*)()>(&QWidget::update));
    qDebug() << "connect(medianSetted) success:" << success;


}


// void CartesianAxis::update() {
//     qDebug() << "update() called!";
//     QWidget::update();
// }

void CartesianGrid::addPoint(double x, double y) {
    points.append(QPointF(x, y));
    emit pointsUpdated();
    qDebug() << "Signal pointsUpdated() emitted";

}


void CartesianAxis::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    center = {width() / 2, height() / 2};
    cartesianGrid->center = center;
    triangleManager->center = center;

}

QVector<QPointF> TriangleManager::filterTrianglePoints(const QVector<QPointF>& points) {
    QVector<QPointF> filteredPoints;

    for (const QPointF& point : points) {
        if ((std::abs(point.x() - triangle.A.x()) < EPS && std::abs(point.y() - triangle.A.y()) < EPS) ||
            (std::abs(point.x() - triangle.B.x()) < EPS && std::abs(point.y() - triangle.B.y()) < EPS) ||
            (std::abs(point.x() - triangle.C.x()) < EPS && std::abs(point.y() - triangle.C.y()) < EPS)) {
            filteredPoints.append(point);
        }
    }
    return filteredPoints;
}


void CartesianGrid::setPoints(const QVector<QPointF>& newPoints) { 
    points = newPoints;
}



void CartesianAxis::deleteNotTriangle() {
    QVector<QPointF> filteredPoints = triangleManager->filterTrianglePoints(cartesianGrid->getPoints());
    cartesianGrid->setPoints(filteredPoints);
    update();
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

    isAutoScaled = true;

    qDebug() << "Computed scale: " << scale;
};


QPointF convertMathToPix(const QPointF& mathPoint, double scale, Triangle triangle, Center center) {

    double midX = (triangle.A.x() + triangle.B.x() + triangle.C.x()) / 3.0;
    double midY = (triangle.A.y() + triangle.B.y() + triangle.C.y()) / 3.0;
    return QPointF(center.x + static_cast<int>((mathPoint.x() - midX) * scale)
                  ,center.y - static_cast<int>((mathPoint.y() - midY) * scale));
}



void TriangleManager::formTriangle(QPainter& painter, int widgetWidth, int widgetHeight, double scale) {
    QPointF A = convertMathToPix(triangle.A, scale, triangle, center);
    QPointF B = convertMathToPix(triangle.B, scale, triangle, center);
    QPointF C = convertMathToPix(triangle.C, scale, triangle, center);

    painter.setPen(Qt::red);
    painter.drawLine(A, B);
    painter.drawLine(B, C);
    painter.drawLine(A, C);

    painter.setPen(Qt::green);
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



void TriangleManager::formMedian(QPainter& painter, double scale) {
    QPointF startMedian = convertMathToPix(median.startMedian, scale, triangle, center);
    QPointF endMedian = convertMathToPix(median.endMedian, scale, triangle, center);

    painter.drawLine(startMedian, endMedian);
}


void CartesianGrid::formAxis(QPainter& painter) {

    int step = 20;
    int tickLength = 5;
    painter.setPen(Qt::black);
    painter.drawLine(0, center.y, center.x * 2, center.y);
    painter.drawLine(center.x, 0, center.x, center.y * 2);

    painter.drawText(center.x + 5, 15, "Y");
    painter.drawText(center.x * 2 - 15, center.y - 5, "X");
    painter.drawText(center.x + 5, center.y + 15, "0");


    for (int i = center.x + step, value = 1; i < center.x * 2; i += step, ++value) {
        painter.drawLine(i, center.y - tickLength, i, center.y + tickLength);
        painter.drawText(i - 10, center.y + 20, QString::number(value));
    }
    for (int i = center.x - step, value = -1; i > 0; i -= step, --value) {
        painter.drawLine(i, center.y - tickLength, i, center.y + tickLength);
        painter.drawText(i - 10, center.y + 20, QString::number(value));
    }
    for (int i = center.y - step, value = 1; i > 0; i -= step, ++value) {
        painter.drawLine(center.x - tickLength, i, center.x + tickLength, i);
        painter.drawText(center.x + 10, i + 5, QString::number(value));
    }
    for (int i = center.y + step, value = -1; i < center.y * 2; i += step, --value) {
        painter.drawLine(center.x - tickLength, i, center.x + tickLength, i);
        painter.drawText(center.x + 10, i + 5, QString::number(value));
    }
}


void CartesianGrid::formDot(QPainter& painter, QPointF point, int numVertex) {
        double x = center.x + point.x() * defaultScale;
        double y = center.y - point.y() * defaultScale;
        QPointF p = {x, y};
        painter.drawEllipse(p, 3, 3);
        painter.drawText(p.x() + 5, p.y() + 5, QString::number(numVertex));
}



void CartesianGrid::formDots(QPainter& painter, Triangle triangle, double defaultScale) {
    painter.setPen(Qt::white);
    int countVertex = 1;

    if (triangle.isValid) {
        formDot(painter, triangle.A, triangle.vertexes.numA);
        formDot(painter, triangle.B, triangle.vertexes.numB);
        formDot(painter, triangle.C, triangle.vertexes.numC);
    }
    else {
        for (const QPointF &point : points) {
            double x = center.x + point.x() * defaultScale;
            double y = center.y - point.y() * defaultScale;
            QPointF p = {x, y};
            painter.drawEllipse(p, 3, 3);
            painter.drawText(p.x() + 5, p.y() + 5, QString::number(countVertex++));
        }
    }
}


void CartesianAxis::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    // qDebug() << "init scale " << scale;

    qDebug() << "paintEvent triggered";
    // qDebug() << "Points: " << cartesianGrid->getPoints();
    // qDebug() << "Size points "<< cartesianGrid->points.size();
    // for (int i = 0; i < cartesianGrid->points.size(); i++)
    // {
    //     qDebug() << "Point : " << cartesianGrid->points[i].x() << " " << cartesianGrid->points[i].y();
    // }
    // qDebug() << "Triangle A:" << triangleManager->triangle.A;
    // qDebug() << "Triangle B:" << triangleManager->triangle.B;
    // qDebug() << "Triangle C:" << triangleManager->triangle.C;

    QPainter painter(this);
    cartesianGrid->formAxis(painter);
    cartesianGrid->formDots(painter, triangleManager->triangle, defaultScale);

    autoScale();

    if (triangle.isValid) {
        painter.setPen(Qt::red);
        triangleManager->formTriangle(painter, width(), height(), scale);
        painter.setPen(Qt::blue);
        triangleManager->formMedian(painter, scale);
    }


}


void CartesianGrid::deletePointAt(const double x, const double y) {
    for (int i = 0; i < points.size(); i++) {
        if (std::abs(x - points[i].x()) <= EPS && std::abs(y - points[i].y()) <= EPS) {
            points.removeAt(i);
            emit pointDeleted();
            break;
        }
    }
}

void TriangleManager::setTriangle(Triangle import_triangle) {
    qDebug() << "Triangle set: " << import_triangle.A << import_triangle.B << import_triangle.C;
    triangle = import_triangle;
    triangle.isValid = true;
    qDebug() << "Stored Triangle: " << triangle.A << triangle.B << triangle.C;
    emit triangleSetted();
}

void TriangleManager::setMedian(Median import_median) {
    median = import_median;
    emit medianSetted();
}


QVector<QPointF> CartesianGrid::getPoints() {
    return points;
}


void CartesianAxis::mousePressEvent(QMouseEvent *event) {

    double x = (event->x() - center.x) / scale;
    double y = (center.y - event->y()) / scale;

    double pointRadius = 5.0;

    QVector<QPointF>points = cartesianGrid->getPoints();

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


void TriangleManager::resetShapes() {
    triangle.reset();
    median.reset();
}

void CartesianAxis::handleDelete() {
    triangleManager->resetShapes();
    resetScale();
    update();
}


void CartesianGrid::deletePoints() {
    points.clear();
    emit allPointsDeleted();
}


CartesianAxis::~CartesianAxis() {
    delete triangleManager;
    delete cartesianGrid;
}
