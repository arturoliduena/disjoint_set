CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = my_program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# g++ -std=c++11 -o p main.cpp 
# g++ -std=c++11 -Wall *.cpp *.h -o my_program

