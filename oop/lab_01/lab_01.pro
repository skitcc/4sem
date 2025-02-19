QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    draw.cpp \
    edges.cpp \
    points.cpp \
    figure.cpp \
    points_operations.cpp \
    ./errorModule/errors.cpp 

HEADERS += \
    mainwindow.h \
    draw.h \
    edges.h \
    points.h \
    figure.h \
    points_operations.h \
    base_samples.h \
    ./errorModule/errors.h 
    

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
