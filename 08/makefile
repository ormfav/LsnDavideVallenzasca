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
MISCFC := $(LIB)Misc/
DATBLK := $(LIB)DataBlocking/
PNTLIB := $(LIB)Point/
MRTLIB := $(LIB)Metropolis/


#User interface
.PHONY: all clean 081 082

all: $(BIN)081.x $(BIN)082.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

081: $(BIN)081.x
082: $(BIN)082.x

#Rules
$(BIN)081.x: $(OBJ)081.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)081.o: $(SRC)081.cpp $(INC)081.h $(wildcard $(CFG)081*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h $(PNTLIB)$(INC)point.h $(MRTLIB)$(INC)metropolis.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/081.cpp -o $@

$(BIN)082.x: $(OBJ)082.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)082.o: $(SRC)082.cpp $(INC)082.h $(wildcard $(CFG)082*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h $(PNTLIB)$(INC)point.h $(MRTLIB)$(INC)metropolis.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/082.cpp -o $@

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


