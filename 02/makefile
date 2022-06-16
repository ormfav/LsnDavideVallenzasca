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
.PHONY: all clean 021 022 

all: $(BIN)021.x $(BIN)022.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

021: $(BIN)021.x
022: $(BIN)022.x

#Rules
$(BIN)021.x: $(OBJ)021.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)021.o: $(SRC)021.cpp $(wildcard $(CFG)021*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/021.cpp -o $@

$(BIN)022.x: $(OBJ)022.o $(RANDOM)$(OBJ)random.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)022.o: $(SRC)022.cpp $(INC)022.h $(wildcard $(CFG)022*) $(RANDOM)$(INC)random.h $(MISCFC)$(INC)misc.h $(DATBLK)$(INC)datablocking.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/022.cpp -o $@

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


