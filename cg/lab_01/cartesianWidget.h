#ifndef CARTESIANWIDGET_H
#define CARTESIANWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

#define EPS 1e-6

class CartesianAxis : public QWidget {
    Q_OBJECT
public:
    CartesianAxis(QWidget *widget = nullptr);
    void addPoint(double x, double y);
    void deletePoints(void);
    void deletePointAt(double x, double y);
    void setTriangle(const QPointF& a, const QPointF& b, const QPointF& c);
    void setMedian(const QPointF& a, const QPointF& b);
    void deleteNotTriangle(void);
    void autoScale(void);

private:
    QVector<QPointF> points;
    QVector<QPointF> triangle_points;
    QVector<QPointF> median_points;
    double scale;
    double offsetX;
    double offsetY;

signals:
    void pointClicked(double x, double y);
    void pointToEdit(double x, double y);
    void pointToDelete(double x, double y);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};



#endif