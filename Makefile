# Compiler
CXX = g++
ifeq ($(ARCH), arm64)
	CXX = aarch64-linux-gnu-g++
endif

# Compiler flags
CXXFLAGS = -Wall -std=c++17

# Source files
SOURCES = FuncA.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = program

# Main target
all: $(TARGET)

# Linking executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compiling object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning up
clean:
	rm -f $(OBJECTS) $(TARGET)

