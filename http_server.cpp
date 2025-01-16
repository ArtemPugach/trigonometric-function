#include <httplib.h>
#include <cmath>
#include "FuncA.h"  // Включаємо клас для обчислення тригонометричних функцій
#include <chrono>
#include <vector>
#include <algorithm>

int main() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        // Створюємо масив для обчислення значень функцій
        FuncA func;
        int n = 70000;  // Розмір масиву
        std::vector<double> results;

        // Обчислюємо значення для масиву
        for (int i = 0; i < n; ++i) {
            results.push_back(func.calculate(1.0, i + 1));  // Викликаємо метод calculate
        }

        // Починаємо вимір часу сортування
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(results.begin(), results.end());  // Сортуємо масив
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Відправляємо відповідь
        res.set_content("Elapsed time: " + std::to_string(elapsed.count()) + " seconds", "text/plain");
    });

    svr.listen("localhost", 8080);  // Запускаємо сервер на localhost:8080
    return 0;
}
