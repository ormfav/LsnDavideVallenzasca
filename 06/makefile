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
.PHONY: all clean 061 

all: $(BIN)061.x 

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"

061: $(BIN)061.x

#Rules
$(BIN)061.x: $(OBJ)061.o $(RANDOM)$(OBJ)random.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)061.o: $(SRC)061.cpp $(INC)061.h $(RANDOM)$(INC)random.h | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/061.cpp -o $@

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


