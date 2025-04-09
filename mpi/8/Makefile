CXX = mpic++
CXXFLAGS = -Wall -O2
TARGET = main
SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	mpirun -np 2 ./$(TARGET)

clean:
	rm -f $(TARGET) 