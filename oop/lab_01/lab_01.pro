QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    draw.cpp \
    edges_operations.cpp \
    points_operations.cpp \
    figure_operations.cpp \
    single_point_operations.cpp \
    errors.cpp \
    actions.cpp

HEADERS += \
    mainwindow.h \
    draw.h \
    edges_operations.h \
    points_operations.h \
    figure_operations.h \
    single_point_operations.cpp \
    errors.h \
    actions.h
    

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
