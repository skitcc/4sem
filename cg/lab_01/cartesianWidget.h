#ifndef CARTESIANWIDGET_H
#define CARTESIANWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

#define EPS 1e-6


#include "structs.h"




class TriangleManager : public QObject{
    Q_OBJECT

public:
    explicit TriangleManager(QObject *parent = nullptr);
    void formTriangle(QPainter& painter, int widgetWidth, int widgetHeight, double scale);
    void formMedian(QPainter& painter, double scale);
    void setTriangle(Triangle import_triangle);
    void setMedian(Median import_median);
    QVector<QPointF> filterTrianglePoints(const QVector<QPointF>& points);
    void resetShapes();

signals:
    void triangleSetted();
    void medianSetted();



public:
    Triangle triangle;
    Median median;
    Center center;
};


class CartesianGrid :public QObject{
    Q_OBJECT

public:
    explicit CartesianGrid(QObject *parent = nullptr);
    void formAxis(QPainter& painter);
    void formDots(QPainter& painter, Triangle triangle, double defaultScale);
    void formDot(QPainter& painter, QPointF point, int numVertex);
    void deletePoints();
    void deletePointAt(double x, double y);
    void addPoint(double x, double y);
    void setPoints(const QVector<QPointF>& newPoints);
    QVector<QPointF> getPoints();

signals:
    void pointsUpdated();
    void pointDeleted();
    void allPointsDeleted();

public:
    QVector<QPointF> points;
    Center center;
    double defaultScale = 20.0;
};


class CartesianAxis : public QWidget {
    Q_OBJECT
public:
    CartesianAxis(QWidget *widget = nullptr);
    void autoScale(void);
    void deleteNotTriangle();
    void resetScale();
    void handleDelete();


private:
    TriangleManager *triangleManager;
    CartesianGrid *cartesianGrid;

    Triangle triangle;
    Median median;
    double scale;
    double defaultScale = 20.0;    
    bool isAutoScaled = false;
    Center center;

    
signals:
    void pointClicked(double x, double y);
    void pointToEdit(double x, double y);
    void pointToDelete(double x, double y);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

// public slots:
//     void updateCanvas() { update(); }


~CartesianAxis();

};





#endif