#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QGridLayout>
#include <QCheckBox>

#include "G:/QtProjects/QCustomPlot/qcustomplot/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCustomPlot* customPlot;

    void drawNewton();
    void drawLagrange();
    void drawFunction();
    void drawSpline();

    double getFunction(double x);
    double getNewthon(double x);
    double getLagrange(double x);
    double getSpline(double x);
    void calcSpline();

    int f;
    const double A = -5;
    const double B = 5;
    const int N = 11;
    QVector<double> coef;
    QVector<double> arr;

    QGridLayout *layout;
    QCheckBox *function;
    QCheckBox *newthon;
    QCheckBox *lagrange;
    QCheckBox *spline;
    QCheckBox *type;

public slots:
    void checkBox_checked();
    void type_checked();
};

#endif // MAINWINDOW_H
