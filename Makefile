TARGET=test_yash

CXX ?= g++
CXXFLAGS=-Wall -Werror -Wextra
CPPFLAGS=
LDFLAGS=

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -rf  $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean


