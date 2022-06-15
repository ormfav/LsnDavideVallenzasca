CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11

.PHONY: all clean

BIN := bin/
OBJ := obj/
SRC := src/
INC := include/
CFG := in/
LIB := ../lib/

RANDOM := $(LIB)Random/
MISCFC := $(LIB)Misc/
DATBLK := $(LIB)DataBlocking/

all: $(BIN)011.x $(BIN)012.x $(BIN)013.x

$(BIN)011.x: $(OBJ)011.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)011.o: $(SRC)011.cpp $(wildcard $(CFG)011*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/011.cpp -o $@

$(BIN)012.x: $(OBJ)012.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)012.o: $(SRC)012.cpp $(wildcard $(CFG)012*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/012.cpp -o $@

$(BIN)013.x: $(OBJ)013.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)013.o: $(SRC)013.cpp $(wildcard $(CFG)013*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/013.cpp -o $@

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

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"
