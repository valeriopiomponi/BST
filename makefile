EXE = $(SRC:.cc=.exe)

CXX = c++
CXXFLAGS = -I include -g -std=c++14 -Wall -Wextra

SRC = main.cc 

OBJ = ${SRC:.cc=.o}

all: $(EXE)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXE): $(OBJ)
	$(CXX) $^ -o $(EXE) $(LFLAGS)

main.o: $(LIB) 

clean:
	rm -rf  main.o $(EXE) */*~ *~ 

.PHONY: clean all


