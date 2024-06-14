#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

using namespace std;

double sumSquares(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += (i + 1) * x[i] * x[i];
    }
    return sum;
}

double step2(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += pow(xi + 0.5, 2);
    }
    return sum;
}

double quartic(const vector<double>& x) {
    static thread_local mt19937 rng(random_device{}());
    static thread_local uniform_real_distribution<double> dist(0.0, 1.0);
    
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += (i + 1) * pow(x[i], 4) + dist(rng);
    }
    return sum;
}

double powell(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() / 4; ++i) {
        double term1 = pow(x[4 * i - 3] + 10 * x[4 * i - 2], 2);
        double term2 = 5 * pow(x[4 * i - 1] + x[4 * i], 2);
        double term3 = pow(x[4 * i - 2] + x[4 * i - 1], 4);
        double term4 = 10 * pow(x[4 * i - 3] + x[4 * i], 4);
        sum += term1 + term2 + term3 + term4;
    }
    return sum;
}

double rosenbrock(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i) {
        sum += 100 * pow(x[i + 1] - x[i] * x[i], 2) + pow(x[i] - 1, 2);
    }
    return sum;
}

double dixonPrice(const vector<double>& x) {
    double term1 = pow(x[0] - 1, 2);
    double sum = term1;
    for (size_t i = 1; i < x.size(); ++i) {
        sum += i * pow(2 * x[i] * x[i] - x[i - 1], 2);
    }
    return sum;
}

double schwefel1_2(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double innerSum = 0.0;
        for (size_t j = 0; j <= i; ++j) {
            innerSum += x[j];
        }
        sum += innerSum * innerSum;
    }
    return sum;
}

double schwefel2_20(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += fabs(xi);
    }
    return sum;
}

double schwefel2_21(const vector<double>& x) {
    double maxVal = fabs(x[0]);
    for (double xi : x) {
        maxVal = max(maxVal, fabs(xi));
    }
    return maxVal;
}

double rastrigin(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += xi * xi - 10 * cos(2 * M_PI * xi) + 10;
    }
    return sum;
}

double griewank(const vector<double>& x) {
    double sum = 0.0;
    double product = 1.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += x[i] * x[i] / 4000.0;
        product *= cos(x[i] / sqrt(i + 1));
    }
    return sum - product + 1;
}

double csendes(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += pow(xi, 6) * (2 + sin(1 / xi));
    }
    return sum;
}

double colville(const vector<double>& x) {
    return 100 * pow(x[1] - pow(x[0], 2), 2) + pow(x[0] - 1, 2) + pow(x[2] - 1, 2) +
           90 * pow(x[3] - x[2], 2) + 10.1 * (pow(x[1] - 1, 2) + pow(x[3] - 1, 2)) + 19.8 * (x[1] - 1) * (x[3] - 1);
}

double easom(const vector<double>& x) {
    return -cos(x[0]) * cos(x[1]) * exp(-pow(x[0] - M_PI, 2) - pow(x[1] - M_PI, 2));
}

double michalewicz(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum += sin(x[i]) * pow(sin((i + 1) * pow(x[i], 2) / M_PI), 20);
    }
    return -sum;
}

double shekel(const vector<double>& x) {
    const double a[10][4] = {
        {4.0, 4.0, 4.0, 4.0}, {1.0, 1.0, 1.0, 1.0}, {8.0, 8.0, 8.0, 8.0}, {6.0, 6.0, 6.0, 6.0},
        {3.0, 7.0, 3.0, 7.0}, {2.0, 9.0, 2.0, 9.0}, {5.0, 5.0, 3.0, 3.0}, {8.0, 1.0, 8.0, 1.0},
        {6.0, 2.0, 6.0, 2.0}, {7.0, 3.6, 7.0, 3.6}
    };
    const double c[10] = {0.1, 0.2, 0.2, 0.4, 0.4, 0.6, 0.3, 0.7, 0.5, 0.5};
    
    double sum = 0.0;
    for (int i = 0; i < 10; ++i) {
        double inner_sum = 0.0;
        for (int j = 0; j < 4; ++j) {
            inner_sum += pow(x[j] - a[i][j], 2);
        }
        sum += 1.0 / (inner_sum + c[i]);
    }
    return -sum;
}

double schwefel2_4(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += pow(xi - 1, 2) + pow(xi - xi, 2);
    }
    return sum;
}

double schwefel(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += -xi * sin(sqrt(fabs(xi)));
    }
    return sum;
}

double schaffer(const vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i) {
        double xi = x[i];
        double xj = x[i + 1];
        sum += 0.5 + (pow(sin(sqrt(xi * xi + xj * xj)), 2) - 0.5) / pow(1 + 0.001 * (xi * xi + xj * xj), 2);
    }
    return sum;
}

double alpine(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += fabs(xi * sin(xi) + 0.1 * xi);
    }
    return sum;
}

double ackley(const vector<double>& x) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (double xi : x) {
        sum1 += xi * xi;
        sum2 += cos(2 * M_PI * xi);
    }
    double n = static_cast<double>(x.size());
    return -20.0 * exp(-0.2 * sqrt(sum1 / n)) - exp(sum2 / n) + 20.0 + M_E;
}

double sphere(const vector<double>& x) {
    double sum = 0.0;
    for (double xi : x) {
        sum += xi * xi;
    }
    return sum;
}

double schwefel2_22(const vector<double>& x) {
    double sum = 0.0;
    double product = 1.0;
    for (double xi : x) {
        sum += fabs(xi);
        product *= fabs(xi);
    }
    return sum + product;
}
