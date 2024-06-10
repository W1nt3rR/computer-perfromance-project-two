#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <omp.h>
#include <iomanip>
#include <functional>
#include <chrono>
#include "functions.cpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Firefly Algorithm parameters
const int population_size = 50;
const int max_generations = 500;
const double alpha = 0.5;      // Randomness strength
const double beta0 = 1.0;      // Attractiveness constant
const double gammaCoeff = 1.0; // Absorption coefficient

// Other Parameters
const int numberOfRuns = 30;
const int minimumThreads = 1;
const int numberOfThreads = omp_get_max_threads();

// Table formatting
const char separator = ' ';
const int nameWidth = 18;
const int numWidth = 16;

struct FunctionBenchmark
{
    int dim;
    double min_range;
    double max_range;
    string name;
    function<double(const vector<double> &)> benchmark;
};

const vector<FunctionBenchmark> functionBenchmarks = {
    {60, -10, 10, "sumSquares", sumSquares},
    {60, -100, 100, "step2", step2},
    {60, -1.28, 1.28, "quartic", quartic},
    {60, -4, 5, "powell", powell},
    {60, -30, 30, "rosenbrock", rosenbrock},
    {60, -10, 10, "dixonPrice", dixonPrice},
    {60, -100, 100, "schwefel1_2", schwefel1_2},
    {60, -100, 100, "schwefel2_20", schwefel2_20},
    {60, -100, 100, "schwefel2_21", schwefel2_21},
    {60, -5.12, 5.12, "rastrigin", rastrigin},
    {60, -600, 600, "griewank", griewank},
    {60, -1, 1, "csendes", csendes},
    {4, -10, 10, "colville", colville},
    {2, -100, 100, "easom", easom},
    {5, 0, M_PI, "michalewicz", michalewicz},
    {4, 0, 10, "shekel", shekel},
    {60, 0, 10, "schwefel2_4", schwefel2_4},
    {60, -500, 500, "schwefel", schwefel},
    {60, -100, 100, "schaffer", schaffer},
    {30, -10, 10, "alpine", alpine},
    {30, -32, 32, "ackley", ackley},
    {30, -5.12, 5.12, "sphere", sphere},
    {30, -10, 10, "schwefel2_22", schwefel2_22}};

// Function to generate random double between min and max using a thread-safe generator
double randomDouble(double min, double max, mt19937 &rng, uniform_real_distribution<> &dist)
{
    return min + dist(rng) * (max - min);
}

// Firefly Algorithm
double fireflyAlgorithm(int dim, double min_range, double max_range, function<double(const vector<double> &)> benchmark, int numThreads)
{
    omp_set_num_threads(numThreads);

    vector<vector<double>> population(population_size, vector<double>(dim));
    vector<double> fitness(population_size);

    // Initialize population and fitness
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < population_size; ++i)
    {
        mt19937 rng(random_device{}() + omp_get_thread_num());
        uniform_real_distribution<> dist(0.0, 1.0);

        for (int j = 0; j < dim; ++j)
        {
            population[i][j] = randomDouble(min_range, max_range, rng, dist);
        }
        fitness[i] = benchmark(population[i]);
    }

    for (int gen = 0; gen < max_generations; ++gen)
    {
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < population_size; ++i)
        {
            mt19937 rng(random_device{}() + omp_get_thread_num());
            uniform_real_distribution<> dist(0.0, 1.0);

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
                        population[i][k] += beta * (population[j][k] - population[i][k]) + alpha * (randomDouble(0, 1, rng, dist) - 0.5);
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

template <typename T>
void printElement(T t, const int &width)
{
    cout << left << setw(width) << setfill(separator) << t;
    cout.flush();
}

int main()
{
    // Table header
    printElement("Function", nameWidth);
    for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
    {
        printElement(to_string(threads) + (threads == 1 ? " Thread" : " Threads"), numWidth);
    }
    cout << endl;
    cout << endl;

    // Run the Firefly Algorithm for each function
    for (const auto &funcBenchmark : functionBenchmarks)
    {
        string function_name = funcBenchmark.name;
        auto benchmark = funcBenchmark.benchmark;
        int dim = funcBenchmark.dim;
        double min_range = funcBenchmark.min_range;
        double max_range = funcBenchmark.max_range;

        printElement(function_name, nameWidth);

        for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
        {
            auto start = chrono::high_resolution_clock::now();

            for (int j = 0; j < numberOfRuns; ++j)
            {
                fireflyAlgorithm(dim, min_range, max_range, benchmark, threads);
            }

            auto end = chrono::high_resolution_clock::now();

            chrono::duration<double> elapsed = end - start;

            printElement(elapsed / numberOfRuns, numWidth);
        }

        cout << endl;
    }

    cout << endl;
    cout << endl;

    return 0;
}
