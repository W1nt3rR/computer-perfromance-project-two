#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <omp.h>
#include <iomanip>
#include "functions.cpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Firefly Algorithm parameters
const int population_size = 20;
const int max_generations = 100;
const double alpha = 0.5;  // Randomness strength
const double beta0 = 1.0;  // Attractiveness constant
const double gammaCoeff = 1.0;  // Absorption coefficient

// Function to generate random double between min and max
double randomDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}

// Function to benchmark
double benchmark(const vector<double> &x, int function_id)
{
    switch (function_id)
    {
    case 1:
        return sumSquares(x);
    case 2:
        return step2(x);
    case 3:
        return quartic(x);
    case 4:
        return powell(x);
    case 5:
        return rosenbrock(x);
    case 6:
        return dixonPrice(x);
    case 7:
        return schwefel1_2(x);
    case 8:
        return schwefel2_20(x);
    case 9:
        return schwefel2_21(x);
    case 10:
        return rastrigin(x);
    case 11:
        return griewank(x);
    case 12:
        return csendes(x);
    case 13:
        return colville(x);
    case 14:
        return easom(x);
    case 15:
        return michalewicz(x);
    case 16:
        return shekel(x);
    case 17:
        return schwefel2_4(x);
    case 18:
        return schwefel(x);
    case 19:
        return schaffer(x);
    case 20:
        return alpine(x);
    case 21:
        return ackley(x);
    case 22:
        return sphere(x);
    case 23:
        return schwefel2_22(x);
    default:
        return numeric_limits<double>::infinity();
    }
}

// Firefly Algorithm
double fireflyAlgorithm(int dim, double min_range, double max_range, int function_id, int numThreads) {

    // Postavljanje broja niti za OpenMP
    omp_set_num_threads(numThreads);

    srand(static_cast<unsigned>(time(0)));
    
    vector<vector<double>> population(population_size, vector<double>(dim));
    vector<double> fitness(population_size);

    // Initialize population
#pragma omp parallel for
    for (int i = 0; i < population_size; ++i) {
        for (int j = 0; j < dim; ++j) {
            population[i][j] = randomDouble(min_range, max_range);
        }
        fitness[i] = benchmark(population[i], function_id);
    }

    for (int gen = 0; gen < max_generations; ++gen) {
#pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < population_size; ++i) {
            for (int j = 0; j < population_size; ++j) {
                if (fitness[i] > fitness[j]) {
                    double r = 0.0;
                    for (int k = 0; k < dim; ++k) {
                        r += pow(population[i][k] - population[j][k], 2);
                    }
                    r = sqrt(r);

                    double beta = beta0 * exp(-gammaCoeff * r * r);
                    for (int k = 0; k < dim; ++k) {
                        population[i][k] += beta * (population[j][k] - population[i][k]) + alpha * (randomDouble(0, 1) - 0.5);
                        population[i][k] = min(max(population[i][k], min_range), max_range);
                    }
                    fitness[i] = benchmark(population[i], function_id);
                }
            }
        }
    }

    auto min_element_it = min_element(fitness.begin(), fitness.end());
    int best_index = distance(fitness.begin(), min_element_it);

    return fitness[best_index];
}

int main()
{
    // Define the functions parameters
    vector<int> dims = {256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 60, 4, 2, 5, 4, 256, 256, 256, 30, 30, 30, 30};
    vector<double> min_ranges = {-10, -100, -1.28, -4, -30, -10, -100, -100, -100, -5.12, -600, -1, -10, -100, 0, 0, 0, -500, -100, -10, -32, -5.12, -10};
    vector<double> max_ranges = {10, 100, 1.28, 5, 30, 10, 100, 100, 100, 5.12, 600, 1, 10, 100, M_PI, 10, 10, 500, 100, 10, 32, 5.12, 10};

    for (int numberOfThreads = 1; numberOfThreads <= 16; numberOfThreads *= 2)
    {
        cout << "Number of threads: " << numberOfThreads << endl;

        for (int i = 1; i <= 23; ++i)
        {
            int numberOfRepeats = 10;
            double result = 0.0;

            for (int j = 0; j < numberOfRepeats; ++j)
            {
                result += fireflyAlgorithm(dims[i - 1], min_ranges[i - 1], max_ranges[i - 1], i, numberOfThreads);
            }

            cout << "Function " << i << " : " << std::fixed << std::setprecision(2) << result / numberOfRepeats << endl;
        }

        cout << endl;
    }

    return 0;
}