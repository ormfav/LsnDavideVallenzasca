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
.PHONY: all clean 072 074

all: $(BIN)072.x $(BIN)074.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

072: $(BIN)072.x
074: $(BIN)074.x

#Rules
$(BIN)072.x: $(OBJ)072.o $(RANDOM)$(OBJ)random.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)072.o: $(SRC)072.cpp $(INC)07.h $(RANDOM)$(INC)random.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/072.cpp -o $@

$(BIN)074.x: $(OBJ)074.o $(RANDOM)$(OBJ)random.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)074.o: $(SRC)074.cpp $(INC)07.h $(RANDOM)$(INC)random.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/074.cpp -o $@

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


