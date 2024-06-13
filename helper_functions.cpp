#include <iostream>
#include <iomanip>
#include <string>
#include <random>

using namespace std;

const char separator = ' ';

/**
 *
 */
bool isNotPowerOfTwo(int n)
{
    if (n == 0)
    {
        return true;
    }
    return n & (n - 1);
}

/**
 * Function to generate random double between min and max using a thread-safe generator
 */
double randomDouble(double min, double max, mt19937 &rng, uniform_real_distribution<> &dist)
{
    return min + dist(rng) * (max - min);
}
/**
 * Print an element with a specific width
 */
template <typename T>
void printElement(T t, const int &width)
{
    cout << left << setw(width) << setfill(separator) << t;
    cout.flush();
}

/**
 * Print an element with a specific width and precision
 */
template <typename T>
void printElementPrecise(T t, const int &width, int precision = 4)
{
    // Save the original state of std::cout
    ios originalState(nullptr);
    originalState.copyfmt(cout);

    cout << left << fixed << setprecision(precision) << setw(width) << setfill(separator) << t;
    cout.flush();

    // Reset std::cout to its original state
    cout.copyfmt(originalState);
}

/**
 * Calculate the average of a vector of doubles
 */
double calculateAverage(const std::vector<double> &vec)
{
    if (vec.empty())
        return 0.0; // Guard against division by zero

    double sum = accumulate(vec.begin(), vec.end(), 0.0);
    double average = sum / vec.size();

    return average;
}
