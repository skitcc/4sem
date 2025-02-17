#include "transformationWidget.h"


Transformation::Transformation(QWidget *parent) : QWidget(parent) {
    // setMinimumSize(1200, 1200);
    // setMaximumSize(800, 800);
    setShapePoints();
}



void Transformation::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    centerX = width() / 2;
    centerY = height() / 2;
}


void Transformation::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    formAxis(painter, scaleX, scaleY);
    drawShape(painter);
}


void Transformation::saveState() {
    historyEllipses.push(ellipses);
    historyTriangles.push(triangles);
    historyLines.push(lines);
    historyAngles.push(lastRotationAngle);
}


void Transformation::undo() {
    if (!historyEllipses.empty() && !historyTriangles.empty() && !historyLines.empty()) {
        ellipses = historyEllipses.top();
        triangles = historyTriangles.top();
        lines = historyLines.top();

        historyEllipses.pop();
        historyTriangles.pop();
        historyLines.pop();
        historyAngles.pop();

        update();
    }
}



void Transformation::pixToMath(const PixelPoint pxPoint, MathPoint& mathPoint, const PixelPoint center) {
    mathPoint.x = pxPoint.x - center.x;
    mathPoint.y = center.y - pxPoint.y;
}


void Transformation::mathToPix(const MathPoint mathPoint, PixelPoint& pxPoint, const PixelPoint center) {
    pxPoint.x = mathPoint.x + center.x;
    pxPoint.y = center.y - mathPoint.y;
}


void Transformation::pixToMath(const int xPix, const int yPix, double& xMath, double& yMath) {
    xMath = xPix - centerX;
    yMath = centerY - yPix;
}


void Transformation::mathToPix(const double xMath, const double yMath, int& xPix, int& yPix) {
    xPix = xMath + centerX;
    yPix = centerY- yMath;
}




void Transformation::setShapePoints() {
    ellipses.clear();
    triangles.clear();
    lines.clear();

    double headXMath, headYMath;
    pixToMath(centerX, centerY, headXMath, headYMath);
    ellipses.push_back({headXMath, headYMath, 1.0, 80, 100});

    std::vector<std::pair<int, int>> pixelEyes = {
        {centerX - 30, centerY - 30},
        {centerX + 30, centerY - 30}
    };
    for (const auto& [xPix, yPix] : pixelEyes) {
        double xMath, yMath;
        pixToMath(xPix, yPix, xMath, yMath);
        ellipses.push_back({xMath, yMath, 1.0, 15, 10, 0.0});
    }

    std::vector<std::pair<int, int>> pixelNose = {
        {centerX, centerY - 10},
        {centerX - 15, centerY + 10},
        {centerX + 15, centerY + 10}
    };
    for (const auto& [xPix, yPix] : pixelNose) {
        double xMath, yMath;
        pixToMath(xPix, yPix, xMath, yMath);
        triangles.push_back({xMath, yMath, 1.0});
    }

    std::vector<std::pair<int, int>> pixelMouth = {
        {centerX - 25, centerY + 40},
        {centerX + 25, centerY + 40},
        {centerX, centerY + 55}
    };
    for (const auto& [xPix, yPix] : pixelMouth) {
        double xMath, yMath;
        pixToMath(xPix, yPix, xMath, yMath);
        triangles.push_back({xMath, yMath, 1.0});
    }

    for (int i = -40; i <= 40; i += 15) {
        double xStartMath, yStartMath, xEndMath, yEndMath;
        pixToMath(centerX + i, centerY - 90, xStartMath, yStartMath);
        pixToMath(centerX + i, centerY - 70, xEndMath, yEndMath);
        lines.push_back({xStartMath, yStartMath, 1.0, xEndMath, yEndMath, 1.0});
    }


    update();
}

void Transformation::transponseEllipses(std::vector<std::vector<double>>transponseMatrix) {

    for (auto& point : ellipses) {
        
        std::vector<double> newPoint(3, 0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += point[j] * transponseMatrix[j][i];
            }
        }
        newPoint.push_back(point[3]);
        newPoint.push_back(point[4]);
        point = newPoint;
    }
}



void Transformation::transponseLines(std::vector<std::vector<double>>transponseMatrix) {

    for (auto& point : lines) {
        
        std::vector<double> newPointStart(3, 0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPointStart[i] += point[j] * transponseMatrix[j][i];
            }
        }

        std::vector<double> newPointEnd(3, 0);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++){ 
                newPointEnd[i] += point[j + 3] * transponseMatrix[j][i];
            }
        }
        point[0] = newPointStart[0];
        point[1] = newPointStart[1];
        point[3] = newPointEnd[0];
        point[4] = newPointEnd[1];
    }
}


void Transformation::transponseTriangles(std::vector<std::vector<double>>transponseMatrix) {

    for (auto& point : triangles) {
        
        std::vector<double> newPoint(3, 0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += point[j] * transponseMatrix[j][i];
            }
        }
        point = newPoint;
    }
}




void Transformation::transponse(double dx, double dy) {

    saveState();
    dx = (dx > 0 ? 1 : -1) * std::abs(dx) * 20 * scaleX;
    dy = (dy > 0 ? 1 : -1) * std::abs(dy) * 20 * scaleY;

    qDebug() << "dx dy : " << dx << dy;


    const std::vector<std::vector<double>> transponseMatrix = {
        {1, 0, 0},
        {0, 1, 0},
        {dx, dy, 1}
    };

    transponseEllipses(transponseMatrix);
    transponseTriangles(transponseMatrix);
    transponseLines(transponseMatrix);

}

void Transformation::scaleEllipses(double cx, double cy, std::vector<std::vector<double>> scaleMatrix) {
    for (auto& point : ellipses) {
        std::vector<double> newPoint(3, 0);

        double xLocal = point[0] - cx;
        double yLocal = point[1] - cy;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += scaleMatrix[i][j] * (j < 2 ? (j == 0 ? xLocal : yLocal) : 1);
            }
        }

        point[0] = newPoint[0] + cx;
        point[1] = newPoint[1] + cy;
        point[3] *= scaleMatrix[0][0];
        point[4] *= scaleMatrix[1][1];
    }
}


void Transformation::scaleLines(double cx, double cy, std::vector<std::vector<double>> scaleMatrix) {
    for (auto& point : lines) {
        std::vector<double> newPointStart(3, 0);
        std::vector<double> newPointEnd(3, 0);

        double xStartLocal = point[0] - cx;
        double yStartLocal = point[1] - cy;

        double xEndLocal = point[3] - cx;
        double yEndLocal = point[4] - cy;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPointStart[i] += scaleMatrix[i][j] * (j < 2 ? (j == 0 ? xStartLocal : yStartLocal) : 1);
                newPointEnd[i] += scaleMatrix[i][j] * (j < 2 ? (j == 0 ? xEndLocal : yEndLocal) : 1);
            }
        }

        point[0] = newPointStart[0] + cx;
        point[1] = newPointStart[1] + cy;
        point[3] = newPointEnd[0] + cx;
        point[4] = newPointEnd[1] + cy;
    }
}


void Transformation::scaleTriangles(double cx, double cy, std::vector<std::vector<double>> scaleMatrix) {
    for (auto& point : triangles) {
        std::vector<double> newPoint(3, 0);

        double xLocal = point[0] - cx;
        double yLocal = point[1] - cy;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += scaleMatrix[i][j] * (j < 2 ? (j == 0 ? xLocal : yLocal) : 1);
            }
        }

        point[0] = newPoint[0] + cx;
        point[1] = newPoint[1] + cy;
    }
}



void Transformation::scale(double kx, double ky, double cx, double cy) {

    saveState();
    scaleX *= kx;
    scaleY *= ky;

    cx = (cx > 0 ? 1 : -1) * std::abs(cx) * 20 * scaleX;
    cy = (cy > 0 ? 1 : -1) * std::abs(cy) * 20 * scaleY;

    const std::vector<std::vector<double>> scaleMatrix = {
        {kx, 0, 0},
        {0, ky, 0},
        {0, 0, 1}
    };

    scaleEllipses(cx, cy, scaleMatrix);
    scaleTriangles(cx, cy, scaleMatrix);
    scaleLines(cx, cy, scaleMatrix);
}

void Transformation::rotateEllipses(double rx, double ry, std::vector<std::vector<double>> rotationMatrix, double angle) {
    for (auto& point : ellipses) {
        std::vector<double> newPoint(3, 0);

        double xLocal = point[0] - rx;
        double yLocal = point[1] - ry;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += rotationMatrix[i][j] * (j < 2 ? (j == 0 ? xLocal : yLocal) : 1);
            }
        }

        point[0] = newPoint[0] + rx;
        point[1] = newPoint[1] + ry;
        point[5] += angle;

    }
}


void Transformation::rotateLines(double rx, double ry, std::vector<std::vector<double>> rotationMatrix) {
    for (auto& point : lines) {
        std::vector<double> newPointStart(3, 0);
        std::vector<double> newPointEnd(3, 0);

        double xStartLocal = point[0] - rx;
        double yStartLocal = point[1] - ry;

        double xEndLocal = point[3] - rx;
        double yEndLocal = point[4] - ry;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPointStart[i] += rotationMatrix[i][j] * (j < 2 ? (j == 0 ? xStartLocal : yStartLocal) : 1);
                newPointEnd[i] += rotationMatrix[i][j] * (j < 2 ? (j == 0 ? xEndLocal : yEndLocal) : 1);
            }
        }

        point[0] = newPointStart[0] + rx;
        point[1] = newPointStart[1] + ry;
        point[3] = newPointEnd[0] + rx;
        point[4] = newPointEnd[1] + ry;
    }
}


void Transformation::rotateTriangles(double rx, double ry, std::vector<std::vector<double>> rotationMatrix) {
    for (auto& point : triangles) {
        std::vector<double> newPoint(3, 0);

        double xLocal = point[0] - rx;
        double yLocal = point[1] - ry;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                newPoint[i] += rotationMatrix[i][j] * (j < 2 ? (j == 0 ? xLocal : yLocal) : 1);
            }
        }

        point[0] = newPoint[0] + rx;
        point[1] = newPoint[1] + ry;
    }
}



void Transformation::rotate(double rx, double ry, double angle) {

    saveState();

    qDebug() << angle;

    double newAngle = -angle * M_PI / 180;

    rx = (rx > 0 ? 1 : -1) * std::abs(rx) * 20 * scaleX;
    ry = (ry > 0 ? 1 : -1) * std::abs(ry) * 20 * scaleY;

    qDebug() << rx << ry;

    const std::vector<std::vector<double>> rotationMatrix {
        {cos(newAngle), -sin(newAngle), 0},
        {sin(newAngle), cos(newAngle), 0},
        {0, 0, 1}
    };

    rotateEllipses(rx, ry, rotationMatrix, angle);
    rotateTriangles(rx, ry, rotationMatrix);
    rotateLines(rx, ry, rotationMatrix);
    lastRotationAngle += angle;
}


void Transformation::drawShape(QPainter& painter) {
    painter.setPen(Qt::blue);
    painter.setRenderHint(QPainter::Antialiasing);

    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    for (const auto& ellipse : ellipses) {
        int centerX, centerY;
        mathToPix(ellipse[0], ellipse[1], centerX, centerY);
        int rx = ellipse[3];
        int ry = ellipse[4]; 
        // double angle = ellipse[5] * 180.0 / M_PI;

        painter.save();
        painter.translate(centerX, centerY);
        painter.rotate(ellipse[5]);
        painter.drawEllipse(QPoint(0, 0), rx, ry);
        painter.restore();
    }

    for (size_t i = 0; i < triangles.size(); i += 3) {
        QPolygon triangle;
        for (int j = 0; j < 3; j++) {
            mathToPix(triangles[i + j][0], triangles[i + j][1], x1, y1);
            triangle << QPoint(x1, y1);
        }
        painter.drawPolygon(triangle);
    }

    for (const auto& line : lines) {
        mathToPix(line[0], line[1], x1, y1);
        mathToPix(line[3], line[4], x2, y2);
        painter.drawLine(x1, y1, x2, y2);
    }
}



void Transformation::formAxis(QPainter& painter, double kx, double ky) {

    int base_step = 20;
    int tickLength = 5;

    painter.setPen(Qt::black);
    painter.drawLine(0, centerY, width(), centerY);
    painter.drawLine(centerX, 0, centerX, height());

    painter.drawText(centerX + 5, 15, "Y");
    painter.drawText(width() - 15, centerY - 5, "X");
    painter.drawText(centerX + 5, centerY + 15, "0");

    double stepX = base_step * kx;
    double stepY = base_step * ky;

    for (double x = stepX, value = 1; x < width(); x += stepX, ++value) {
        int x_pix, y_pix;
        mathToPix(x, 0, x_pix, y_pix);
        painter.drawLine(x_pix, centerY - tickLength, x_pix, centerY + tickLength);
        painter.drawText(x_pix - 10, centerY + 20, QString::number(value * scaleX));
    }

    for (double x = -stepX, value = -1; x > -width(); x -= stepX, --value) {
        int x_pix, y_pix;
        mathToPix(x, 0, x_pix, y_pix);
        painter.drawLine(x_pix, centerY - tickLength, x_pix, centerY + tickLength);
        painter.drawText(x_pix - 10, centerY + 20, QString::number(value * scaleX));
    }

    for (double y = stepY, value = 1; y < height(); y += stepY, ++value) {
        int x_pix, y_pix;
        mathToPix(0, y, x_pix, y_pix);
        painter.drawLine(centerX - tickLength, y_pix, centerX + tickLength, y_pix);
        painter.drawText(centerX + 10, y_pix + 5, QString::number(value * scaleY));
    }

    for (double y = -stepY, value = -1; y > -height(); y -= stepY, --value) {
        int x_pix, y_pix;
        mathToPix(0, y, x_pix, y_pix);
        painter.drawLine(centerX - tickLength, y_pix, centerX + tickLength, y_pix);
        painter.drawText(centerX + 10, y_pix + 5, QString::number(value * scaleY));
    }
}



