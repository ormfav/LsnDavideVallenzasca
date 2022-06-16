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

#User interface
.PHONY: all clean 031 

all: $(BIN)031.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

031: $(BIN)031.x

#Rules
$(BIN)031.x: $(OBJ)031.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)031.o: $(SRC)031.cpp $(wildcard $(CFG)031*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/031.cpp -o $@

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


