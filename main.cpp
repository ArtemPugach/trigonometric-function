#include <iostream>
#include "FuncA.h"

int main() {
	FuncA func;  // Використовуємо конструктор без параметрів
	int n = 5;  // Кількість елементів для обчислення
	std::cout << "Result: " << func.FuncA(1.0, n) << std::endl;  // Викликаємо метод для обчислення
	return 0;
}

