#include "FuncA.h"
#include <cmath>

FuncA::FuncA() {} // Default constructor definition

double FuncA::calculate(double x, int n) {
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result += std::pow(-1, i) * std::pow(x, 2 * i + 1) / (2 * i + 1);
    }
    return result;
}
