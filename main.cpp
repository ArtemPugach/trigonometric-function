#include <iostream>
#include "FuncA.h"

int main() {
    FuncA func; // Using default constructor
    int n = 5; // Number of terms for calculation
    std::cout << "Result: " << func.calculate(1.0, n) << std::endl; // Calling the method for calculation
    return 0;
}
