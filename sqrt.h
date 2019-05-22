#ifndef SQRT_H
#define SQRT_H
#include <QVector>
#include <cmath>
namespace SqrtMt {
  QVector<QVector<double>> transp(QVector<QVector<double>> a) {
    int i, j;
    int n = a.size();
    QVector<QVector<double>> b(n);
    for (i = 0; i < n; i++)
    {
      b[i] = QVector<double>(n);
    }
    for (i = 0; i < n; i++)
    {
      for (j = 0; j < n; j++)
        b[j][i] = a[i][j];
    }
    return b;
  }

  int sign(double z)
  {
    if (z > 0)
      return 1;
    else
      return -1;
  }

  void solve(const QVector<QVector<double>> a, const QVector<double> b, QVector<double>& x)
  {
    int i, j, k;
    int n = a.size();
    bool flag = true;
    double kst;
    QVector<QVector<double>> at(n), rab(n), s(n),d(n);
    QVector<double> nev(n), y(n), b1(n);


    for (i = 0; i < n; i++)
    {
      at[i] = QVector<double>(n);
      rab[i] = QVector<double>(n);
      s[i] = QVector<double>(n);
      d[i] = QVector<double>(n);
    }

    for (i = 0; i < n; ++i)
    {
      for (j = 0; j < n; j++)
      {
        if (a[i][j] != a[j][i]) {
          flag = false;
        }
        d[i][j] = 0;
        s[i][j] = 0;
      }
    }
    if (!flag)
    {
      {
        for (i = 0; i < n; i++)
        {
          for (j = 0; j < n; j++)
            at[j][i] = a[i][j];
        }
        for (i = 0; i < n; i++)
        {
          for (j = 0; j < n; j++)
          {
            rab[i][j] = 0;
            b1[i] = 0;
            for (k = 0; k < n; k++)
            {
              rab[i][j] += at[i][k] * a[k][j];
              b1[i] += at[i][k] * b[k];
            }
          }
        }
      }
    }
    else
    {
      for (i = 0; i < n; i++)
      {
        b1[i] = b[i];
        for (j = 0; j < n; j++)
          rab[i][j] = a[i][j];
      }
    }
    for (int i = 0; i < n; i++)
    {
      for (int k = 0; k < (i + 1); k++)
      {
        double sum = 0;
        for (int j = 0; j < k; j++)
          sum += s[i][j] * s[k][j] * d[j][j];
        if (i == k)
        {
          s[i][k] = sqrt(rab[i][i] - sum);
          d[i][k] = sign(rab[i][i] - sum);
        }
        else
          s[i][k] = (1.0 / s[k][k] * (rab[i][k] - sum));
      }
    }
    s = transp(s);
    for (i = 0; i < n; i++)
    {
      double sum = 0;
      for (int k = 0; k <= i - 1; k++)
        sum += y[k] * s[k][i];
      y[i] = (b1[i] - sum) / s[i][i];

    }

    for (i = n - 1; i >= 0; i--)
    {
      double sum = 0;
      for (int k = i + 1; k <= n - 1; k++)
        sum += s[i][k] * x[k];
      x[i] = (y[i] - sum) / s[i][i];
    }
  }
}
#endif // SQRT_H
