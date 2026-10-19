CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic

APP := mediatheque

.PHONY: all app run test clean

all: app

app: $(APP)

$(APP): main.cpp Application.cpp $(wildcard Models/*.cpp)
	$(CXX) $(CXXFLAGS) main.cpp -o $(APP)

run: $(APP)
	./$(APP)

test: $(APP)
	bash tests/integration.sh

clean:
	rm -f $(APP) mediatheque_app mediatheque_demo
