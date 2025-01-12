#include <iostream>
#include "FuncA.h"

int main() {
    FuncA func;
    int n = 5; 
    std::cout << "Result: " << func.FuncA(1.0, n) << std::endl;  // Виклик функції з параметром n
    return 0;
}

