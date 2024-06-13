#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <chrono>

using namespace std;

struct FunctionBenchmark
{
    int dim;
    double min_range;
    double max_range;
    string name;
    function<double(const vector<double> &)> benchmark;
};

struct Benchmark
{
    int threadCount;
    string functionName;
    chrono::duration<double> time;
    double averageResult;
    double bestResult;
};