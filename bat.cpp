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
#include "benchmark_functions.cpp"

using namespace std;
using namespace std::chrono;

// Bat Algorithm Implementation
class BatAlgorithm
{
public:
    BatAlgorithm(int numBats, int dimensions, int maxIterations, double minFreq, double maxFreq,
                 double loudness, double pulseRate, double lowerBound, double upperBound, int numThreads,
                 double (*benchmarkFunction)(const vector<double> &))
        : numBats(numBats), dimensions(dimensions), maxIterations(maxIterations), minFreq(minFreq),
          maxFreq(maxFreq), loudness(loudness), pulseRate(pulseRate), lowerBound(lowerBound),
          upperBound(upperBound), numThreads(numThreads), benchmarkFunction(benchmarkFunction)
    {
        // Initialize random number generators
        rng.seed(random_device{}());
        randDouble = uniform_real_distribution<double>(0.0, 1.0);
        randPosition = uniform_real_distribution<double>(lowerBound, upperBound);
        randFrequency = uniform_real_distribution<double>(minFreq, maxFreq);
        randVelocity = uniform_real_distribution<double>(-abs(upperBound - lowerBound), abs(upperBound - lowerBound));
    }

    // Run the Bat Algorithm
    double run()
    {
        // Initialize bats
        vector<vector<double>> bats(numBats, vector<double>(dimensions));
        vector<vector<double>> velocities(numBats, vector<double>(dimensions, 0.0));
        vector<double> frequencies(numBats, 0.0);
        vector<double> fitness(numBats, numeric_limits<double>::max());

        // Initialize global best
        vector<double> globalBestPosition(dimensions);
        double globalBestFitness = numeric_limits<double>::max();

        // Randomly initialize bats' positions and velocities
        for (int i = 0; i < numBats; ++i)
        {
            for (int j = 0; j < dimensions; ++j)
            {
                bats[i][j] = randPosition(rng);
                velocities[i][j] = randVelocity(rng);
            }
            fitness[i] = benchmarkFunction(bats[i]);
            if (fitness[i] < globalBestFitness)
            {
                globalBestFitness = fitness[i];
                globalBestPosition = bats[i];
            }
        }

        // Main loop of Bat Algorithm
        for (int t = 0; t < maxIterations; ++t)
        {
            // Parallelize the loop using OpenMP
#pragma omp parallel for num_threads(numThreads)
            for (int i = 0; i < numBats; ++i)
            {
                // Update frequency
                frequencies[i] = minFreq + (maxFreq - minFreq) * randDouble(rng);

                // Update velocity and position
                for (int j = 0; j < dimensions; ++j)
                {
                    velocities[i][j] += (bats[i][j] - globalBestPosition[j]) * frequencies[i];
                    bats[i][j] += velocities[i][j];

                    // Ensure the new position is within the boundaries
                    if (bats[i][j] < lowerBound)
                    {
                        bats[i][j] = lowerBound;
                    }
                    else if (bats[i][j] > upperBound)
                    {
                        bats[i][j] = upperBound;
                    }
                }

                // Pulse rate and loudness based update
                if (randDouble(rng) > pulseRate)
                {
                    for (int j = 0; j < dimensions; ++j)
                    {
                        bats[i][j] = globalBestPosition[j] + randVelocity(rng) * loudness;
                        if (bats[i][j] < lowerBound)
                        {
                            bats[i][j] = lowerBound;
                        }
                        else if (bats[i][j] > upperBound)
                        {
                            bats[i][j] = upperBound;
                        }
                    }
                }

                // Evaluate the fitness of new solutions
                double newFitness = benchmarkFunction(bats[i]);

                // Update if the solution improves
                if (newFitness < fitness[i] && randDouble(rng) < loudness)
                {
                    fitness[i] = newFitness;
                    if (newFitness < globalBestFitness)
                    {
                        globalBestFitness = newFitness;
                        globalBestPosition = bats[i];
                    }
                }
            }
        }
        return globalBestFitness;
    }

private:
    int numBats;
    int dimensions;
    int maxIterations;
    double minFreq;
    double maxFreq;
    double loudness;
    double pulseRate;
    double lowerBound;
    double upperBound;
    int numThreads;
    double (*benchmarkFunction)(const vector<double> &);

    // Random number generators
    mt19937 rng;
    uniform_real_distribution<double> randDouble;
    uniform_real_distribution<double> randPosition;
    uniform_real_distribution<double> randFrequency;
    uniform_real_distribution<double> randVelocity;
};

int main()
{
    // Parameters for Bat Algorithm
    int numBats = 50;
    int dimensions = 30;
    int maxIterations = 5000; // Increase iterations
    double minFreq = 0.0;
    double maxFreq = 2.0;
    double loudness = 0.5;
    double pulseRate = 0.5;
    double lowerBound = -100.0;
    double upperBound = 100.0;
    int numThreads = 8; // Change this to the desired number of threads

    // List of benchmark functions
    vector<pair<string, double (*)(const vector<double> &)>> functions = {
        {"Sum Squares Function", sumSquaresFunction},
        {"Step 2 Function", step2Function},
        {"Quartic Function", quarticFunction},
        {"Powell Function", powellFunction},
        {"Rosenbrock Function", rosenbrockFunction},
        {"Dixon & Price Function", dixonPriceFunction},
        {"Schwefel 1.2 Function", schwefel12Function},
        {"Schwefel 2.20 Function", schwefel220Function},
        {"Schwefel 2.21 Function", schwefel221Function},
        {"Rastrigin Function", rastriginFunction},
        {"Griewank Function", griewankFunction},
        {"Csendes Function", csendesFunction},
        {"Colville Function", colvilleFunction},
        {"Easom Function", easomFunction},
        {"Michalewicz Function", michalewiczFunction},
        {"Shekel Function", shekelFunction},
        {"Schwefel Function", schwefelFunction},
        {"Schwefel 2.4 Function", schwefel24Function},
        {"Schaffer Function", schafferFunction},
    };

    // Run the Bat Algorithm on each function and print the results
    for (const auto &function : functions)
    {
        auto start = high_resolution_clock::now();

        BatAlgorithm ba(numBats, dimensions, maxIterations, minFreq, maxFreq, loudness, pulseRate, lowerBound, upperBound, numThreads, function.second);

        // Perform 30 runs and calculate the average best solution
        double totalBest = 0.0;
        for (int run = 0; run < 30; ++run)
        {
            totalBest += ba.run();
        }
        double averageBest = totalBest / 30.0;

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        cout << "Function: " << function.first << endl;
        cout << "Average Best Solution: " << averageBest << endl;
        cout << "Execution Time: " << duration << " ms" << endl
             << endl;
    }

    return 0;
}