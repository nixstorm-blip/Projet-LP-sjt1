CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsqlite3

SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = mediatheque

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

re: clean all
