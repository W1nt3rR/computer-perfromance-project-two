#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <omp.h>
#include <iomanip>
#include <functional>
#include "functions.cpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Firefly Algorithm parameters
const int population_size = 20;
const int max_generations = 100;
const double alpha = 0.5;      // Randomness strength
const double beta0 = 1.0;      // Attractiveness constant
const double gammaCoeff = 1.0; // Absorption coefficient

// Function to generate random double between min and max
double randomDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}

// Firefly Algorithm
double fireflyAlgorithm(int dim, double min_range, double max_range, function<double(const vector<double> &)> benchmark, int numThreads)
{

    // Postavljanje broja niti za OpenMP
    omp_set_num_threads(numThreads);

    srand(static_cast<unsigned>(time(0)));

    vector<vector<double>> population(population_size, vector<double>(dim));
    vector<double> fitness(population_size);

    // Initialize population
#pragma omp parallel for
    for (int i = 0; i < population_size; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            population[i][j] = randomDouble(min_range, max_range);
        }
        fitness[i] = benchmark(population[i]);
    }

    for (int gen = 0; gen < max_generations; ++gen)
    {
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < population_size; ++i)
        {
            for (int j = 0; j < population_size; ++j)
            {
                if (fitness[i] > fitness[j])
                {
                    double r = 0.0;
                    for (int k = 0; k < dim; ++k)
                    {
                        r += pow(population[i][k] - population[j][k], 2);
                    }
                    r = sqrt(r);

                    double beta = beta0 * exp(-gammaCoeff * r * r);
                    for (int k = 0; k < dim; ++k)
                    {
                        population[i][k] += beta * (population[j][k] - population[i][k]) + alpha * (randomDouble(0, 1) - 0.5);
                        population[i][k] = min(max(population[i][k], min_range), max_range);
                    }
                    fitness[i] = benchmark(population[i]);
                }
            }
        }
    }

    auto min_element_it = min_element(fitness.begin(), fitness.end());
    int best_index = distance(fitness.begin(), min_element_it);

    return fitness[best_index];
}

const char separator = ' ';
const int nameWidth = 18;
const int numWidth = 16;

template <typename T>
void printElement(T t, const int &width)
{
    cout << left << setw(width) << setfill(separator) << t;
}

int main()
{
    // Define the functions parameters
    vector<int> dims = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 60, 4, 2, 5, 4, 256, 256, 256, 30, 30, 30, 30};
    vector<double> min_ranges = {-10, -100, -1.28, -4, -30, -10, -100, -100, -100, -5.12, -600, -1, -10, -100, 0, 0, 0, -500, -100, -10, -32, -5.12, -10};
    vector<double> max_ranges = {10, 100, 1.28, 5, 30, 10, 100, 100, 100, 5.12, 600, 1, 10, 100, M_PI, 10, 10, 500, 100, 10, 32, 5.12, 10};

    // Vector to store function names and their corresponding benchmark functions
    vector<pair<string, function<double(const vector<double> &)>>> functionBenchmarks = {
        {"sumSquares", sumSquares}, {"step2", step2}, {"quartic", quartic}, {"powell", powell}, {"rosenbrock", rosenbrock}, {"dixonPrice", dixonPrice}, {"schwefel1_2", schwefel1_2}, {"schwefel2_20", schwefel2_20}, {"schwefel2_21", schwefel2_21}, {"rastrigin", rastrigin}, {"griewank", griewank}, {"csendes", csendes}, {"colville", colville}, {"easom", easom}, {"michalewicz", michalewicz}, {"shekel", shekel}, {"schwefel2_4", schwefel2_4}, {"schwefel", schwefel}, {"schaffer", schaffer}, {"alpine", alpine}, {"ackley", ackley}, {"sphere", sphere}, {"schwefel2_22", schwefel2_22}};

    // table header
    printElement("Function", nameWidth);
    printElement("1 Thread", numWidth);
    printElement("2 Threads", numWidth);
    printElement("4 Threads", numWidth);
    printElement("8 Threads", numWidth);
    printElement("16 Threads", numWidth);
    printElement("Best Performance", numWidth);
    cout << endl;
    cout << endl;

    for (int i = 0; i < functionBenchmarks.size(); ++i)
    {

        const auto &funcPair = functionBenchmarks[i];
        string function_name = funcPair.first;
        auto benchmark = funcPair.second;

        auto results = vector<double>();

        for (int numberOfThreads = 1; numberOfThreads <= 16; numberOfThreads *= 2)
        {
            int numberOfRepeats = 10;
            double result = 0.0;

            for (int j = 0; j < numberOfRepeats; ++j)
            {
                result += fireflyAlgorithm(dims[i], min_ranges[i], max_ranges[i], benchmark, numberOfThreads);
            }

            results.push_back(result / numberOfRepeats);
        }

        auto min_element_it = min_element(results.begin(), results.end());

        int best_index = distance(results.begin(), min_element_it);

        printElement(function_name, nameWidth);
        for (double result : results)
        {
            printElement(result, numWidth);
        }

        int numThreads = static_cast<int>(pow(2, best_index));
        std::string threadText = numThreads == 1 ? " Thread" : " Threads";
        printElement(std::to_string(numThreads) + threadText, numWidth);

        cout << endl;
    }

    cout << endl
         << endl;

    return 0;
}
