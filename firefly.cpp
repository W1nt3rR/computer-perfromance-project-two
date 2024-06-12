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
const double alpha = 0.05;      // Randomness strength
const double beta0 = 1.0;      // Attractiveness constant
const double gammaCoeff = 0.5; // Absorption coefficient

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

    // Initialize vectors
    vector<vector<double>> population(population_size, vector<double>(dim));
    vector<double> fitness(population_size);

    // Initialize population and fitness
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < population_size; ++i)
    {
        // Random number generator
        mt19937 rng(random_device{}() + omp_get_thread_num());
        // Uniformly distributes those numbers between the specified range
        // Now each number has the same chance of being chosen
        uniform_real_distribution<> dist(0.0, 1.0);

        for (int j = 0; j < dim; ++j)
        {
            // Set position in the search space for each firefly (a vector of random values in a specific range)
            population[i][j] = randomDouble(min_range, max_range, rng, dist);
        }
        // Calculate the fitness values for each firefly with its initial position
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
                    int intersect = 0;
                    int unionSize = dim;
                    for (int k = 0; k < dim; ++k)
                    {
                        if (population[i][k] == population[j][k])
                        {
                            intersect++;
                        }
                        else
                        {
                            unionSize++;
                        }
                    }
                    // Calculate the distance between two vectors using cosine similarity
                    double r = 1.0 - (double)intersect / unionSize;

                    // alpha - randomness
                    // beta - attractivness     
                    // This line calculates the attractiveness of firefly j to firefly i based on their distance r    
                    double beta = beta0 * exp(-gammaCoeff * r * r);
                    for (int k = 0; k < dim; ++k)
                    {
                        // Multiplying attractivness with the direction from firefly i to firefly j 
                        // population[j][k] - population[i][k] (distance between the two, if negative firefly i is to the right else j is) 
                        // Then we add the randomness to prevent local optima
                        population[i][k] += beta * (population[j][k] - population[i][k]) + alpha * (randomDouble(0, 1, rng, dist) - 0.5);
                        // Ensures that the new position of firefly i in dimension k remains within the specified bounds
                        population[i][k] = min(max(population[i][k], min_range), max_range);
                    }
                    // Reevalute fitness for firefly i
                    fitness[i] = benchmark(population[i]);
                }
            }
        }
    }

    // Identifying the index of the superior firefly 
    auto min_element_it = min_element(fitness.begin(), fitness.end());
    int best_index = distance(fitness.begin(), min_element_it);

    return fitness[best_index];
}

struct Benchmark
{
    int threadCount;
    string functionName;
    chrono::duration<double> time;
};

template <typename T>
void printElement(T t, const int &width)
{
    cout << left << setw(width) << setfill(separator) << t;
    cout.flush();
}

bool isNotPowerOfTwo(int n) {
    if (n == 0) {
        return true;
    }
    return n & (n - 1);
}

Benchmark executeBenchmark(const FunctionBenchmark &functionBenchmark, int threads)
{
    string function_name = functionBenchmark.name;
    auto benchmark = functionBenchmark.benchmark;
    int dim = functionBenchmark.dim;
    double min_range = functionBenchmark.min_range;
    double max_range = functionBenchmark.max_range;

    auto start = chrono::high_resolution_clock::now();

    for (int j = 0; j < numberOfRuns; ++j)
    {
        fireflyAlgorithm(dim, min_range, max_range, benchmark, threads);
    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    Benchmark b;
    b.threadCount = threads;
    b.functionName = function_name;
    b.time = elapsed / numberOfRuns;

    return b;
}

int main()
{
    // Table header
    printElement("Function", nameWidth);
    for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
    {
        printElement(to_string(threads) + (threads == 1 ? " Thread" : " Threads"), numWidth);
    }

    bool isNotPowerOfTwoThreads = isNotPowerOfTwo(numberOfThreads);

    // If the max threads is not a power of two, add it to the table
    if (isNotPowerOfTwoThreads)
    {
        printElement(to_string(numberOfThreads) + " Threads", numWidth);
    }

    printElement("Best", numWidth);
    cout << endl;
    cout << endl;

    // Run the Firefly Algorithm for each function
    for (const auto &funcBenchmark : functionBenchmarks)
    {
        vector<Benchmark> benchmarkData;
        string function_name = funcBenchmark.name;

        printElement(function_name, nameWidth);

        // Execute the benchmark for each number of threads
        for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
        {
            Benchmark bench = executeBenchmark(funcBenchmark, threads);
            printElement(bench.time, numWidth);
            benchmarkData.push_back(bench);
        }

        // If the max threads is not a power of two, run a benchmark for it
        if (isNotPowerOfTwoThreads)
        {
            Benchmark bench = executeBenchmark(funcBenchmark, numberOfThreads);
            printElement(bench.time, numWidth);
            benchmarkData.push_back(bench);
        }

        // Find the best time
        auto min_element_it = min_element(benchmarkData.begin(), benchmarkData.end(), [](const Benchmark &a, const Benchmark &b) {
            return a.time < b.time;
        });

        int best_index = distance(benchmarkData.begin(), min_element_it);

        printElement(to_string(benchmarkData[best_index].threadCount) + (benchmarkData[best_index].threadCount == 1 ? " Thread" : " Threads"), numWidth);

        cout << endl;
    }

    cout << endl;
    cout << endl;

    return 0;
}
