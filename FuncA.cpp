#include "FuncA.h"
#include <cmath>  


double FuncA::FuncA(double x) {
    double result = 0;  // Ініціалізуємо змінну для зберігання результату
    // Обчислюємо суму перших 3 елементів
    for (int i = 0; i < 3; ++i) {
        // Додаємо до результату поточний елемент послідовності
        result += std::pow(-1, i) * std::pow(x, 2 * i + 1) / (2 * i + 1);
    }
    return result;  // Повертаємо обчислений результат
}

