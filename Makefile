CXX = g++
CXXFLAGS = -Wall -std=c++11
SOURCES = FuncA.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

