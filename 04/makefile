#Aliases
CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11

BIN := bin/
OBJ := obj/
SRC := src/
INC := include/
CFG := in/
LIB := ../lib/

RANDOM := $(LIB)Random/

#User interface
.PHONY: all clean 041 

all: $(BIN)041.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

041: $(BIN)041.x

#Rules
$(BIN)041.x: $(OBJ)041.o $(RANDOM)$(OBJ)random.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)041.o: $(SRC)041.cpp $(INC)041.h $(wildcard $(CFG)*) $(RANDOM)$(INC)random.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/041.cpp -o $@

$(RANDOM)$(OBJ)random.o: 
	$(MAKE) -C $(RANDOM)

$(MISCFC)$(OBJ)misc.o: 
	$(MAKE) -C $(MISCFC)

$(BIN):
	@echo "Creating $@"
	@mkdir -p $@

$(OBJ):
	@echo "Creating $@"
	@mkdir -p $@


