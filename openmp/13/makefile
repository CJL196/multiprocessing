CXX = g++
TARGET = main
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))
CXXFLAGS = -c -Wall
OPENMPFLAGS = -fopenmp

$(TARGET): $(OBJ)
	$(CXX) $(OPENMPFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(OPENMPFLAGS) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)