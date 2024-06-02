#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <algorithm>
#include <random>
#include <math.h>
#include "benchmark_functions.cpp"

using namespace std;
using namespace std::chrono;

class FireflyAlgorithm {
public:
    FireflyAlgorithm(int numFireflies, int dimensions, int maxIterations, double alpha, double beta0, double gamma, double lowerBound, double upperBound, int numThreads, double (*benchmarkFunction)(const vector<double>&))
        : numFireflies(numFireflies), dimensions(dimensions), maxIterations(maxIterations), alpha(alpha), beta0(beta0), gamma(gamma), lowerBound(lowerBound), upperBound(upperBound), numThreads(numThreads), benchmarkFunction(benchmarkFunction) {
        rng.seed(random_device{}());
        randDouble = uniform_real_distribution<double>(0.0, 1.0);
        randPosition = uniform_real_distribution<double>(lowerBound, upperBound);
    }

    double run() {
        vector<vector<double>> fireflies(numFireflies, vector<double>(dimensions));
        vector<double> intensities(numFireflies);

        for (int i = 0; i < numFireflies; ++i) {
            for (int j = 0; j < dimensions; ++j) {
                fireflies[i][j] = randPosition(rng);
            }
            intensities[i] = benchmarkFunction(fireflies[i]);
        }

        for (int t = 0; t < maxIterations; ++t) {
            #pragma omp parallel for num_threads(numThreads) schedule(dynamic)
            for (int i = 0; i < numFireflies; ++i) {
                for (int j = 0; j < numFireflies; ++j) {
                    if (intensities[j] < intensities[i]) {
                        update_firefly(fireflies[i], fireflies[j], intensities[i]);
                    }
                }
            }
        }

        auto best_it = min_element(intensities.begin(), intensities.end());
        int best_index = distance(intensities.begin(), best_it);
        return intensities[best_index];
    }

private:
    int numFireflies;
    int dimensions;
    int maxIterations;
    double alpha;
    double beta0;
    double gamma;
    double lowerBound;
    double upperBound;
    int numThreads;
    double (*benchmarkFunction)(const vector<double>&);

    mt19937 rng;
    uniform_real_distribution<double> randDouble;
    uniform_real_distribution<double> randPosition;

    void update_firefly(vector<double>& firefly_i, const vector<double>& firefly_j, double& intensity_i) {
        for (int d = 0; d < dimensions; ++d) {
            double r = abs(firefly_i[d] - firefly_j[d]);
            double beta = beta0 * exp(-gamma * r * r);
            firefly_i[d] = firefly_i[d] + beta * (firefly_j[d] - firefly_i[d]) + alpha * (randDouble(rng) - 0.5);
            firefly_i[d] = min(max(firefly_i[d], lowerBound), upperBound);
        }
        intensity_i = benchmarkFunction(firefly_i);
    }
};

int main() {
    // Parameters for Firefly Algorithm
    int numFireflies = 50;
    int dimensions = 30;
    int maxIterations = 30;
    double alpha = 0.2;
    double beta0 = 1.0;
    double gamma = 1.0;
    double lowerBound = -100.0;
    double upperBound = 100.0;
    int numThreads = 64;

    // List of benchmark functions
    vector<pair<string, double (*)(const vector<double>&)>> functions = {
        {"Gay Sum Squares Function", sumSquaresFunction},
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

    // Run the Firefly Algorithm on each function and print the results
    for (const auto& function : functions) {
        auto start = high_resolution_clock::now();

        FireflyAlgorithm fa(numFireflies, dimensions, maxIterations, alpha, beta0, gamma, lowerBound, upperBound, numThreads, function.second);

        // Perform 30 runs and calculate the average best solution
        double totalBest = 0.0;
        for (int run = 0; run < 30; ++run) {
            totalBest += fa.run();
        }
        double averageBest = totalBest / 30.0;

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start).count();

        cout << "Function: " << function.first << endl;
        cout << "Average Best Solution: " << averageBest << endl;
        cout << "Execution Time: " << duration << " ms" << endl << endl;
    }

    return 0;
}
