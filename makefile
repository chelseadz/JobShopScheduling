CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET   := main
SRCS     := main.cpp Utils.cpp Random.cpp Validation.cpp

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
