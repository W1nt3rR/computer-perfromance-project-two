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
#include <sstream>
#include <fstream>
#include "functions.cpp"
#include "helper_functions.cpp"
#include "firefly.hpp"

using namespace std;

/**
 * Firefly Algorithm parameters
 */
const int population_size = 50;
const int max_generations = 4000;

const double beta0 = 1.0;      // Attractiveness constant
const double gammaCoeff = 0.5; // Absorption coefficient

const double minAlpha = 0.05;
const double maxAlpha = 0.2;
const double alphaIncrement = (maxAlpha - minAlpha) / max_generations;

/**
 * Other Parameters
 */
const int numberOfRuns = 30;
const int minimumThreads = 1;
const int numberOfThreads = omp_get_max_threads();
const bool isNotPowerOfTwoThreads = isNotPowerOfTwo(numberOfThreads);

/**
 * Table formatting
 */
const int nameWidth = 16;
const int numWidth = 14;

/**
 * Function benchmarks
 */
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

/**
 * Firefly Algorithm
 */
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

    double alpha = minAlpha;

    for (int gen = 0; gen < max_generations; ++gen)
    {
        // Update alpha
        alpha += alphaIncrement;

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

/**
 * Execute the benchmark for a specific function and number of threads
 */
Benchmark executeBenchmark(const FunctionBenchmark &functionBenchmark, int threads)
{
    string function_name = functionBenchmark.name;
    auto benchmark = functionBenchmark.benchmark;
    int dim = functionBenchmark.dim;
    double min_range = functionBenchmark.min_range;
    double max_range = functionBenchmark.max_range;

    auto start = chrono::high_resolution_clock::now();

    vector<double> results(numberOfRuns);

    for (int j = 0; j < numberOfRuns; ++j)
    {
        results.push_back(fireflyAlgorithm(dim, min_range, max_range, benchmark, threads));
    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;

    Benchmark b;
    b.threadCount = threads;
    b.functionName = function_name;
    b.time = elapsed / numberOfRuns;
    b.averageResult = calculateAverage(results);
    b.bestResult = *min_element(results.begin(), results.end());

    return b;
}

/**
 * Print the table header
 */
void printTableHeader() {
    printElement("Function", nameWidth);
    for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
    {
        printElement(to_string(threads) + (threads == 1 ? " Thread" : " Threads"), numWidth);
    }

    // If the max threads is not a power of two, add it to the table
    if (isNotPowerOfTwoThreads)
    {
        printElement(to_string(numberOfThreads) + " Threads", numWidth);
    }
}

/**
 * Print the table title
 */
void printTableTitle(string title, int titlePadding = 2, int extraCells = 0)
{
    string newTitle = string(titlePadding, ' ') + title + string(titlePadding, ' ');
    int titleWidth = newTitle.length();

    int tableWidth = nameWidth + (extraCells + 1 + log2(numberOfThreads) - log2(minimumThreads)) * numWidth + (isNotPowerOfTwoThreads ? numWidth : 0);
    int sidePadding = (tableWidth - titleWidth) / 2;

    cout << setfill('=') << setw(sidePadding) << "=";
    cout << setfill(' ') << setw(titleWidth) << newTitle;
    cout << setfill('=') << setw(sidePadding) << "=";
    cout << endl;
}

/**
 * Print the results table
 */
void printResultsTable(vector<vector<Benchmark>> &allBenchmarkData)
{
    printTableTitle("Average Results Table");
    printTableHeader();
    cout << endl;
    cout << endl;

    for (int i = 0; i < allBenchmarkData.size(); i++)
    {
        printElement(functionBenchmarks[i].name, nameWidth);

        for (int j = 0; j < allBenchmarkData[i].size(); j++)
        {
            printElement(allBenchmarkData[i][j].averageResult, numWidth);
        }

        cout << endl;
    }

    cout << endl;
    cout << endl;
}

/**
 * Print the best results table
 */
void printBestResultsTable(vector<vector<Benchmark>> &allBenchmarkData)
{
    printTableTitle("Best Results Table");
    printTableHeader();
    cout << endl;
    cout << endl;

    for (int i = 0; i < allBenchmarkData.size(); i++)
    {
        printElement(functionBenchmarks[i].name, nameWidth);

        for (int j = 0; j < allBenchmarkData[i].size(); j++)
        {
            printElement(allBenchmarkData[i][j].bestResult, numWidth);
        }

        cout << endl;
    }

    cout << endl;
    cout << endl;
}

/**
 * Print the speedup table
 */
void printSpeedupTable(vector<vector<Benchmark>> &allBenchmarkData)
{
    printTableTitle("Speedup Table");
    printTableHeader();
    cout << endl;
    cout << endl;

    for (int i = 0; i < allBenchmarkData.size(); i++)
    {
        printElement(functionBenchmarks[i].name, nameWidth);

        for (int j = 0; j < allBenchmarkData[i].size(); j++)
        {
            double speedup = allBenchmarkData[i][0].time / allBenchmarkData[i][j].time;

            // Print in the format "1.5x"
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(1) << speedup << "x";
            std::string speedupStr = stream.str();
            std::cout << std::left << std::setw(numWidth) << speedupStr;
        }

        cout << endl;
    }

    cout << endl;
    cout << endl;
}

/**
 * Write the benchmark data to CSV files
 */
void writeCSVFiles(vector<vector<Benchmark>> &allBenchmarkData)
{
    string timeCSV = "time.csv";
    string resultCSV = "result.csv";
    string speedupCSV = "speedup.csv";

    ofstream timeFile(timeCSV);
    ofstream resultFile(resultCSV);
    ofstream speedupFile(speedupCSV);

    timeFile << "Function,";
    resultFile << "Function,";
    speedupFile << "Function,";

    for (int threads = minimumThreads; threads <= numberOfThreads; threads *= 2)
    {
        timeFile << to_string(threads) + (threads == 1 ? " Thread" : " Threads") + ",";
        resultFile << to_string(threads) + (threads == 1 ? " Thread" : " Threads") + ",";
        speedupFile << to_string(threads) + (threads == 1 ? " Thread" : " Threads") + ",";
    }

    if (isNotPowerOfTwoThreads)
    {
        timeFile << to_string(numberOfThreads) + " Threads,";
        resultFile << to_string(numberOfThreads) + " Threads,";
        speedupFile << to_string(numberOfThreads) + " Threads,";
    }

    timeFile << endl;
    resultFile << endl;
    speedupFile << endl;

    for (int i = 0; i < allBenchmarkData.size(); i++)
    {
        timeFile << functionBenchmarks[i].name + ",";
        resultFile << functionBenchmarks[i].name + ",";
        speedupFile << functionBenchmarks[i].name + ",";

        for (int j = 0; j < allBenchmarkData[i].size(); j++)
        {
            timeFile << allBenchmarkData[i][j].time.count() << ",";
            resultFile << allBenchmarkData[i][j].averageResult << ",";

            double speedup = allBenchmarkData[i][0].time / allBenchmarkData[i][j].time;
            speedupFile << speedup << ",";
        }

        timeFile << endl;
        resultFile << endl;
        speedupFile << endl;
    }

    timeFile.close();
    resultFile.close();
    speedupFile.close();
}

/**
 * Main function
 */
int main()
{
    // Measure the total execution time
    auto start = chrono::high_resolution_clock::now();

    printTableTitle("Speed Table", 2 , 1);
    printTableHeader();
    printElement("Best", numWidth);
    cout << endl;
    cout << endl;

    vector<vector<Benchmark>> allBenchmarkData;

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
            printElementPrecise(bench.time, numWidth);
            benchmarkData.push_back(bench);
        }

        // If the max threads is not a power of two, run a benchmark for it
        if (isNotPowerOfTwoThreads)
        {
            Benchmark bench = executeBenchmark(funcBenchmark, numberOfThreads);
            printElementPrecise(bench.time, numWidth);
            benchmarkData.push_back(bench);
        }

        allBenchmarkData.push_back(benchmarkData);

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

    // Print the rest of the tables
    printSpeedupTable(allBenchmarkData);
    printResultsTable(allBenchmarkData);
    printBestResultsTable(allBenchmarkData);

    writeCSVFiles(allBenchmarkData);

    // Calculate the total execution time
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Print the total execution time
    cout << "Total execution time: " << elapsed;

    cout << endl;
    cout << endl;

    return 0;
}
