QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TEMPLATE = app
TARGET = interpolation

SOURCES += main.cpp\
         G:/QtProjects/QCustomPlot/qcustomplot/qcustomplot.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
         G:/QtProjects/QCustomPlot/qcustomplot/qcustomplot.h \ \
    sqrt.h

FORMS += \
    mainwindow.ui
