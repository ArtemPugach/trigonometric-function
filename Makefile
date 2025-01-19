# Змінна для архітектури, за замовчуванням - поточна система
ARCH ?= $(shell uname -m)

# Вибір компілятора залежно від архітектури
CXX = g++
ifeq ($(ARCH), aarch64)
    CXX = aarch64-linux-gnu-g++
endif

# Флаги компілятора
CXXFLAGS = -Wall -std=c++17

# Файли проекту
SOURCES = FuncA.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = program

# Головна ціль
all: $(TARGET)

# Правило для створення виконуваного файлу
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Правило для створення об'єктних файлів
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очищення
clean:
	rm -f $(OBJECTS) $(TARGET)

