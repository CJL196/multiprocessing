CXX = gcc
TARGET = main
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
CXXFLAGS = -c -Wall -lpthread

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)