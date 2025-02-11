#ifndef CARTESIANWIDGET_H
#define CARTESIANWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

#define EPS 1e-6


#include "structs.h"




class CartesianAxis : public QWidget {
    Q_OBJECT
public:
    CartesianAxis(QWidget *widget = nullptr);
    void addPoint(double x, double y);
    void deletePoints(void);
    void deletePointAt(double x, double y);
    void setTriangle(Triangle import_triangle);
    void setMedian(Median import_median);
    void deleteNotTriangle(void);
    void autoScale(void);
    QPointF convertPixToMath(QPointF point);
    QPointF convertMathToPix(const QPointF& mathPoint);
    void updateOffsets();
    void formTriangle(QPainter& painter);
    void formMedian(QPainter& painter);
    void formAxis(QPainter& painter);
    void formDots(QPainter& painter);
    void resetScale();
private:
    QVector<QPointF> points;
    Triangle triangle;
    Median median;
    double scale;
    double defaultScale = 20.0;    
    bool isAutoScaled = false;
    double offsetX = 0;
    double offsetY = 0;
    double centerX;
    double centerY;

signals:
    void pointClicked(double x, double y);
    void pointToEdit(double x, double y);
    void pointToDelete(double x, double y);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
};



#endif