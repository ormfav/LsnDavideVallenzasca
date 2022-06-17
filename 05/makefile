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
.PHONY: all clean 051 

all: $(BIN)051.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

051: $(BIN)051.x

#Rules
$(BIN)051.x: $(OBJ)051.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)051.o: $(SRC)051.cpp $(wildcard $(CFG)051*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h $(PNTLIB)$(INC)point.h $(MRTLIB)$(INC)metropolis.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/051.cpp -o $@

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


