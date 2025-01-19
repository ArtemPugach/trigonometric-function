#include <httplib.h>
#include <cmath>
#include "FuncA.h"  // Включаємо клас для обчислення тригонометричних функцій
#include <chrono>
#include <vector>
#include <algorithm>
#include <iostream> // Для виводу діагностичної інформації

int main() {
    httplib::Server svr;

    // Налаштування маршруту для кореневого запиту
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        FuncA func;
        int n = 70000;  // Розмір масиву
        std::vector<double> results;

        // Обчислення значень
        for (int i = 0; i < n; ++i) {
            results.push_back(func.calculate(1.0, i + 1));  // Викликаємо метод calculate
        }

        // Сортування масиву з виміром часу
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(results.begin(), results.end());
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Відповідь із часом сортування
        res.set_content("Elapsed time: " + std::to_string(elapsed.count()) + " seconds", "text/plain");
    });

    // Запуск сервера на 0.0.0.0:8080
    std::cout << "Starting server on http://0.0.0.0:8080" << std::endl;
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "Error: Server failed to start on port 8080!" << std::endl;
        return -1;
    }

    return 0;
}

