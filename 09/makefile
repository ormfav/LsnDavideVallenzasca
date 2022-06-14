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
GENALG := $(LIB)Ga/
MISCFC := $(LIB)Misc/

all: $(BIN)091.x

$(BIN)091.x: $(OBJ)091.o $(RANDOM)$(OBJ)random.o $(GENALG)$(OBJ)individual.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)091.o: $(SRC)091.cpp $(wildcard $(INC)*.h) $(wildcard $(CFG)091*) $(RANDOM)$(INC)random.h $(wildcard $(GENALG)$(INC)*.h) | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/091.cpp -o $@

$(GENALG)$(OBJ)individual.o: 
	$(MAKE) -C $(GENALG)

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
