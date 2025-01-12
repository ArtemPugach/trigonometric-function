#include <iostream>
#include "FuncA.h"

int main() {
    FuncA func;
    int n = 5; 
    std::cout << "Result: " << func.calculate(1.0, n) << std::endl; 
    return 0;
}
