#include <iostream>
#include <cassert>
#include "FuncA.h"

void testFuncA() {
    FuncA func;
    int n = 5;
    double result = func.FuncA(1.0, n);  // Викликаємо функцію з аргументами
    assert(result != 0);  // Перевірка, що результат не нульовий
    std::cout << "Test passed!" << std::endl;
}

int main() {
    testFuncA();  // Викликаємо одиничний тест
    return 0;
}

