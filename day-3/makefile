CXX = g++
CFLAGS = -g -Wall

CPPFILES = main.cpp inputparse.cpp
OBJFILES = $(CPPFILES:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) -c $^ -o $@

clean:
	$(RM) $(TARGET)
	$(RM) $(OBJFILES)
