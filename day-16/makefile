CXX = g++
CFLAGS = -g -Wall -O2

CPPFILES = main.cpp inputparse.cpp
OBJFILES = $(CPPFILES:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) $(TARGET)
	$(RM) $(OBJFILES)
