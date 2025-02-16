#ifndef TRANSFORMATION_WIDGET_H
#define TRANSFORMATION_WIDGET_H


#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>
#include <QDebug>

#include <cmath>
#include <stack>

#define DEFAULT_VECTOR_SIZE 3


struct MathPoint {
    double x, y;
};

struct PixelPoint {
    int x, y;
};


class Transformation : public QWidget {
    Q_OBJECT

public:
    Transformation(QWidget *parent = nullptr);

    void transponse(double dx, double dy);
    void transponseEllipses(std::vector<std::vector<double>>transponseMatrix);
    void transponseTriangles(std::vector<std::vector<double>>transponseMatrix);
    void transponseLines(std::vector<std::vector<double>>transponseMatrix);

    void scale(double kx, double ky, double cx, double cy);
    void scaleEllipses(double cx, double cy, std::vector<std::vector<double>>scalingMatrix);
    void scaleTriangles(double cx, double cy, std::vector<std::vector<double>>scalingMatrix);
    void scaleLines(double cx, double cy, std::vector<std::vector<double>>scalingMatrix);


    void rotate(double rx, double ry, double angle);
    void rotateEllipses(double rx, double ry, std::vector<std::vector<double>> rotationMatrix, double angle);
    void rotateTriangles(double rx, double ry, std::vector<std::vector<double>> rotationMatrix);
    void rotateLines(double rx, double ry, std::vector<std::vector<double>> rotationMatrix);

    void mathToPix(const MathPoint mathPoint, PixelPoint& pxPoint, const PixelPoint center);
    void pixToMath(const PixelPoint pxPoint, MathPoint& mathPoint, const PixelPoint center);


    void pixToMath(const int xPix, const int yPix, double& xMath, double& yMath);
    void mathToPix(const double xMath, const double yMath, int& xPix, int& yPix);


    void drawShape(QPainter& painter);
    void setShapePoints();

    void formAxis(QPainter& painter, double kx, double ky);


    void saveState();
    void undo();

    ~Transformation() override {};

private:
    std::vector<std::vector<double>> triangles;
    std::vector<std::vector<double>> ellipses;
    std::vector<std::vector<double>> lines;


    std::stack<std::vector<std::vector<double>>> historyEllipses;
    std::stack<std::vector<std::vector<double>>> historyTriangles;
    std::stack<std::vector<std::vector<double>>> historyLines;

    double centerX, centerY;
    double scaleX = 1.0, scaleY = 1.0;
    double lastCx = 0.0, lastCy = 0.0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
};



#endif