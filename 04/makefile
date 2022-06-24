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
.PHONY: all clean 042 

all: $(BIN)042.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

042: $(BIN)042.x

#Rules
$(BIN)042.x: $(OBJ)042.o $(RANDOM)$(OBJ)random.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)042.o: $(SRC)042.cpp $(INC)042.h $(wildcard $(CFG)*) $(RANDOM)$(INC)random.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/042.cpp -o $@

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


