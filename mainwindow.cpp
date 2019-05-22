#include "mainwindow.h"
#include <cmath>
#include "sqrt.h"
#include <QDebug>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), customPlot(new QCustomPlot(this)), layout(new QGridLayout), function(new QCheckBox("Початкова функція", this)),
    newthon(new QCheckBox("Поліном Ньютона", this)), lagrange(new QCheckBox("Поліном Лагранжа", this)),
    spline(new QCheckBox("Кубічний сплайн", this)), type(new QCheckBox("Вузли Чебишева", this))
{
    ui->setupUi(this);

    function->setStyleSheet("QCheckBox {"
                            "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "border: none;"
                            "color: blue;"
                            "font-size: 16px;"
                         "}");
    newthon->setStyleSheet("QCheckBox {"
                           "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "border: none;"
                            "color: green;"
                            "font-size: 16px;"
                         "}");
    lagrange->setStyleSheet("QCheckBox {"
                            "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "border: none;"
                            "color: darkMagenta;"
                            "font-size: 16px;"
                         "}");
    spline->setStyleSheet("QCheckBox {"
                            "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "border: none;"
                            "color: red;"
                            "font-size: 16px;"
                         "}");

    type->setStyleSheet("QCheckBox {"
                            "font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "border: none;"
                            "font-size: 16px;"
                         "}");

    type_checked();

    connect(function, &QCheckBox::stateChanged, this, &MainWindow::checkBox_checked);
    connect(newthon, &QCheckBox::stateChanged, this, &MainWindow::checkBox_checked);
    connect(lagrange, &QCheckBox::stateChanged, this, &MainWindow::checkBox_checked);
    connect(spline, &QCheckBox::stateChanged, this, &MainWindow::checkBox_checked);

    connect(type, &QCheckBox::stateChanged, this, &MainWindow::type_checked);


    layout->addWidget(function, 0, 0);
    layout->addWidget(newthon, 0, 1);
    layout->addWidget(lagrange, 0, 2);
    layout->addWidget(spline, 0, 3);
    layout->addWidget(type, 0, 4);

    layout->addWidget(customPlot, 1, 0, 5, 5);

    QWidget * w = new QWidget(this);
    w->setLayout(layout);
    setCentralWidget(w);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::getFunction(double x)
{
    //return m[x];
    return x*sin(3*x);
}

double MainWindow::getNewthon(double x)
{
    double res = 0;
    for(int j = 0; j < arr.size(); ++j){
        double a = 0;

        for(int k = 0; k <= j ; k++){
            double temp = getFunction(arr[k]);

            for(int t = 0; t <= j; t++){
                if(t != k)
                    temp /= arr[k] - arr[t];
            }
            a += temp;
        }

        double n = 1;
        for(int i = 0; i < j; i++){
            n *= x - arr[i];
        }
        res += a*n;
    }
    return res;
}

double MainWindow::getLagrange(double x)
{
    double res = 0;
    for(int j = 0; j < arr.size(); ++j){
        double n = 1;
        for(int i = 0; i < arr.size(); ++i){
            if(i!=j){
                n *= (x - arr[i])/(arr[j] - arr[i]);
            }
        }
        res += getFunction(arr[j])*n;
    }
    return res;
}

void MainWindow::calcSpline()
{
    QVector<double> bSpline;
    bSpline.push_back(getFunction(arr[0]));
    for(int i = 1; i < arr.size() - 1; ++i){
        bSpline.push_back(getFunction(arr[i]));
        bSpline.push_back(getFunction(arr[i]));
    }
    bSpline.push_back(getFunction(arr[arr.size()-1]));

    int size = bSpline.size();
    for(int i = 0; i < size; ++i){
        bSpline.push_back(0);
    }
    for(int i = 0; i < bSpline.size(); ++i){
        qDebug() << bSpline[i];
    }

    QVector<QVector<double>> a;

    for(int i = 0; i < bSpline.size(); ++i){
        a.push_back(QVector<double>());
        for(int j = 0; j < bSpline.size(); ++j){
            a[i].push_back(0.0);
        }
    }

    for(int i = 0; i < arr.size()-1; ++i){
        QVector<double> &first = a[i*2];
        QVector<double> &second = a[i*2+1];

        first[i*4]   = arr[i]*arr[i]*arr[i];
        first[i*4+1] = arr[i]*arr[i];
        first[i*4+2] = arr[i];
        first[i*4+3] = 1.0;

        second[i*4]   = arr[i+1]*arr[i+1]*arr[i+1];
        second[i*4+1] = arr[i+1]*arr[i+1];
        second[i*4+2] = arr[i+1];
        second[i*4+3] = 1.0;
    }
    int middle = size;

    for(int i = 1; i < arr.size() - 1; ++i){
        QVector<double> &first = a[middle + (i-1)*2];
        QVector<double> &second = a[middle + (i-1)*2+1];

        first[(i-1)*4]   = 3*arr[i]*arr[i];
        first[(i-1)*4+1] = 2*arr[i];
        first[(i-1)*4+2] = 1.0;
        first[(i-1)*4+3] = 0.0;
        first[(i-1)*4+4] = -3*arr[i]*arr[i];
        first[(i-1)*4+5] = -2*arr[i];
        first[(i-1)*4+6] = -1.0;
        first[(i-1)*4+7] = 0.0;

        second[(i-1)*4]   = 6*arr[i];
        second[(i-1)*4+1] = 2.0;
        second[(i-1)*4+2] = 0.0;
        second[(i-1)*4+3] = 0.0;
        second[(i-1)*4+4] = -6*arr[i];
        second[(i-1)*4+5] = -2.0;
        second[(i-1)*4+6] = 0.0;
        second[(i-1)*4+7] = 0.0;
    }

    QVector<double> &first = a[a.size()-2];
    QVector<double> &second = a[a.size()-1];

    first[0] = 6*arr[0];
    first[1] = 2.0;

    second[second.size() - 4] = 6*arr[arr.size() - 1];
    second[second.size() - 3] = 2.0;

    for(int i = 0; i < bSpline.size(); ++i){
        coef.push_back(0.0);
    }
    coef = QVector<double>();
    SqrtMt::solve(a, bSpline, coef);
    qDebug() << coef;
}

double MainWindow::getSpline(double x)
{
    bool check = false;
    int index = 1;
    for(;index < arr.size(); ++index){
        if(x <= arr[index]){
            check = true;
            break;
        }
    }

    index--;

    double a = coef[index*4];
    double b = coef[index*4+1];
    double c = coef[index*4+2];
    double d = coef[index*4+3];

    return a*x*x*x+b*x*x+c*x+d;
}

void MainWindow::drawNewton()
{
    QVector<double> x(101), y(101);
    for (int i=0; i<101; ++i)
    {
      x[i] = arr.first()+(arr.last()-arr.first())*i/100.0;
      y[i] = getNewthon(x[i]);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(f)->setData(x, y);
    customPlot->graph(f)->setPen(QPen(Qt::green));
    f++;
}
void MainWindow::drawSpline()
{
    QVector<double> x(101), y(101);
    for (int i=0; i<101; ++i)
    {
      //x[i] = A+(B-A)*i/100.0; // x goes from -1 to 1
      x[i] = arr.first()+(arr.last()-arr.first())*i/100.0;
      y[i] = getSpline(x[i]);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(f)->setData(x, y);
    customPlot->graph(f)->setPen(QPen(Qt::red));
    f++;
}
void MainWindow::drawFunction()
{
    QVector<double> x(101), y(101);
    for (int i=0; i<101; ++i)
    {
      x[i] = A+(B-A)*i/100.0; // x goes from -1 to 1
      y[i] = getFunction(x[i]);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(f)->setData(x, y);
    customPlot->graph(f)->setPen(QPen(Qt::blue));
    f++;
}
void MainWindow::drawLagrange()
{
    QVector<double> x(101), y(101);
    for (int i=0; i<101; ++i)
    {
      x[i] = arr.first()+(arr.last()-arr.first())*i/100.0;
      y[i] = getLagrange(x[i]);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(f)->setData(x, y);
    customPlot->graph(f)->setPen(QPen(Qt::darkMagenta));
    f++;
}

void MainWindow::checkBox_checked()
{
    customPlot->clearGraphs();
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-5, 5);
    customPlot->yAxis->setRange(-5, 5);

    f = 0;

    if(function->isChecked()){
        drawFunction();
    }
    if(newthon->isChecked()){
        drawNewton();
    }
    if(lagrange->isChecked()){
        drawLagrange();
    }
    if(spline->isChecked()){
        drawSpline();
    }
    customPlot->replot();
}

void MainWindow::type_checked()
{
    arr = QVector<double>();
    if(type->isChecked()){
        for(int i = N; i > 0; i--){
            arr.push_back(0.5*(A+B)+0.5*(B-A)*cos((2*i-1)*3.1415926/(2*N)));
        }
    }else{
        for(int i = 0; i < N; i++){
            arr.push_back(A+(B-A)*i/(N-1));
        }
    }
    calcSpline();
    checkBox_checked();
}
































