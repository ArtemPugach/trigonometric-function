# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -I./cpp-httplib

# Source files
SOURCES = http_server.cpp FuncA.cpp
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

