#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <random>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace std::chrono;

// Sum Squares Function
double sumSquaresFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        sum += (i + 1) * x[i] * x[i];
    }
    return sum;
}

// Step 2 Function
double step2Function(const vector<double> &x)
{
    double sum = 0.0;
    for (double xi : x)
    {
        sum += pow(floor(xi) + 0.5, 2);
    }
    return sum;
}

// Quartic Function
double quarticFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        sum += (i + 1) * pow(x[i], 4);
    }
    return sum + static_cast<double>(rand()) / RAND_MAX;
}

// Powell Function
double powellFunction(const vector<double> &x)
{
    double sum = 0.0;
    int n = x.size();
    for (int i = 0; i < n / 4; ++i)
    {
        double term1 = x[4 * i] + 10 * x[4 * i + 1];
        double term2 = x[4 * i + 2] - x[4 * i + 3];
        double term3 = x[4 * i + 1] - 2 * x[4 * i + 2];
        double term4 = x[4 * i] - x[4 * i + 3];
        sum += term1 * term1 + 5 * term2 * term2 + term3 * term3 * term3 * term3 + 10 * term4 * term4 * term4 * term4;
    }
    return sum;
}

// Rosenbrock Function
double rosenbrockFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
        sum += 100 * pow((x[i + 1] - pow(x[i], 2)), 2) + pow((x[i] - 1), 2);
    }
    return sum;
}

// Dixon & Price Function
double dixonPriceFunction(const vector<double> &x)
{
    double sum = pow(x[0] - 1, 2);
    for (size_t i = 1; i < x.size(); ++i)
    {
        sum += i * pow(2 * pow(x[i], 2) - x[i - 1], 2);
    }
    return sum;
}

// Schwefel 1.2 Function
double schwefel12Function(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        double inner_sum = 0.0;
        for (size_t j = 0; j <= i; ++j)
        {
            inner_sum += x[j];
        }
        sum += pow(inner_sum, 2);
    }
    return sum;
}

// Schwefel 2.20 Function
double schwefel220Function(const vector<double> &x)
{
    double sum = 0.0;
    for (double xi : x)
    {
        sum += fabs(xi);
    }
    return sum;
}

// Schwefel 2.21 Function
double schwefel221Function(const vector<double> &x)
{
    return *max_element(x.begin(), x.end(), [](double a, double b)
                        { return fabs(a) < fabs(b); });
}

// Rastrigin Function
double rastriginFunction(const vector<double> &x)
{
    double sum = 10 * x.size();
    for (double xi : x)
    {
        sum += (xi * xi - 10 * cos(2 * M_PI * xi));
    }
    return sum;
}

// Griewank Function
double griewankFunction(const vector<double> &x)
{
    double sum = 0.0;
    double product = 1.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        sum += pow(x[i], 2) / 4000.0;
        product *= cos(x[i] / sqrt(i + 1));
    }
    return sum - product + 1;
}

// Csendes Function
double csendesFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (double xi : x)
    {
        sum += pow(xi, 6) * (2 + sin(1 / xi));
    }
    return sum;
}

// Colville Function
double colvilleFunction(const vector<double> &x)
{
    return 100 * pow(x[1] - pow(x[0], 2), 2) + pow(1 - x[0], 2) + 90 * pow(x[3] - pow(x[2], 2), 2) + pow(1 - x[2], 2) + 10.1 * (pow(x[1] - 1, 2) + pow(x[3] - 1, 2)) + 19.8 * (x[1] - 1) * (x[3] - 1);
}

// Easom Function
double easomFunction(const vector<double> &x)
{
    return -cos(x[0]) * cos(x[1]) * exp(-pow(x[0] - M_PI, 2) - pow(x[1] - M_PI, 2));
}

// Michalewicz Function
double michalewiczFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        sum += sin(x[i]) * pow(sin((i + 1) * x[i] * x[i] / M_PI), 20);
    }
    return -sum;
}

// Shekel Function
double shekelFunction(const vector<double> &x)
{
    // Shekel function constants
    const int m = 10;
    double a[10][4] = {
        {4.0, 4.0, 4.0, 4.0},
        {1.0, 1.0, 1.0, 1.0},
        {8.0, 8.0, 8.0, 8.0},
        {6.0, 6.0, 6.0, 6.0},
        {3.0, 7.0, 3.0, 7.0},
        {2.0, 9.0, 2.0, 9.0},
        {5.0, 5.0, 3.0, 3.0},
        {8.0, 1.0, 8.0, 1.0},
        {6.0, 2.0, 6.0, 2.0},
        {7.0, 3.6, 7.0, 3.6}};
    double c[10] = {0.1, 0.2, 0.2, 0.4, 0.4, 0.6, 0.3, 0.7, 0.5, 0.5};

    double sum = 0.0;
    for (int i = 0; i < m; ++i)
    {
        double sum1 = 0.0;
        for (int j = 0; j < 4; ++j)
        {
            sum1 += pow(x[j] - a[i][j], 2);
        }
        sum += 1.0 / (sum1 + c[i]);
    }
    return -sum;
}

// Schwefel Function
double schwefelFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (double xi : x)
    {
        sum += xi * sin(sqrt(fabs(xi)));
    }
    return 418.9829 * x.size() - sum;
}

// Schwefel 2.4 Function
double schwefel24Function(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i)
    {
        double inner_sum = 0.0;
        for (size_t j = 0; j < i; ++j)
        {
            inner_sum += x[j];
        }
        sum += pow(inner_sum, 2);
    }
    return sum;
}

// Schaffer Function
double schafferFunction(const vector<double> &x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
        double term1 = pow(sin(pow(x[i] * x[i] + x[i + 1] * x[i + 1], 0.5)), 2) - 0.5;
        double term2 = pow(1 + 0.001 * (x[i] * x[i] + x[i + 1] * x[i + 1]), 2);
        sum += 0.5 + term1 / term2;
    }
    return sum;
}