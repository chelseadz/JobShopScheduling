CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET   := main.exe
SRCS     := main.cpp Utils.cpp Random.cpp Validation.cpp Greedy.cpp

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
