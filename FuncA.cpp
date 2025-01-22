#include "FuncA.h"
#include <cmath>

FuncA::FuncA() { }

double FuncA::arctg(double x, int n)
{
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        double term = std::pow(-1.0, i) * std::pow(x, 2 * i + 1) / (2 * i + 1);
        result += term;
    }

    return result;
}

