#include <iostream>
#include <gtest/gtest.h>
#include "FuncA.h"
#include <chrono>

// Тест для перевірки часу обчислень
TEST(ExecutionTimeTest, TimeWithinLimits) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Викликаємо функцію для обчислення
    FuncA func;
    for (int i = 0; i < 20000; ++i) {
        func.calculate(1.0, i + 1); 
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Перевірка, що час виконання між 5 і 20 секунд
    ASSERT_GE(elapsed.count(), 5);  // Час не менший за 5 секунд
    ASSERT_LE(elapsed.count(), 20); // Час не більший за 20 секунд
}

// Тест для перевірки правильності результату
TEST(FuncATest, NonZeroResult) {
    FuncA func;
    int n = 5;
    double result = func.calculate(1.0, n);  // Викликаємо функцію з аргументами

    // Перевірка, що результат не нульовий
    ASSERT_NE(result, 0);  // Перевірка, що результат не дорівнює нулю
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);  // Ініціалізація Google Test
    return RUN_ALL_TESTS();  // Запуск усіх тестів
}

