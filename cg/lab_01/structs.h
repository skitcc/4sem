#ifndef STRUCTS_H
#define STRUCTS_H


#include <QPointF>

struct NumVertexes {
    int numA, numB, numC;
};



struct Triangle {
    QPointF A;
    QPointF B;
    QPointF C;


    NumVertexes vertexes;

    bool isValid;

    Triangle() : A(), B(), C(), isValid(false) {}

    Triangle(QPointF a, QPointF b, QPointF c) 
        : A(a), B(b), C(c), isValid(true) {}

    void reset() {
        A = QPointF();
        B = QPointF();
        C = QPointF();
        isValid = false;
    }
};


struct Median {
    QPointF startMedian;
    QPointF endMedian;
    bool isValid;

    Median() : startMedian(), endMedian(), isValid(false) {}

    Median(QPointF start, QPointF end) 
        : startMedian(start), endMedian(end), isValid(true) {}

    void reset() {
        startMedian = QPointF();
        endMedian = QPointF();
        isValid = false;
    }

};




#endif