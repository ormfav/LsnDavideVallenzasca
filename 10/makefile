CXX = mpicxx
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

all: $(BIN)101.x $(BIN)102.x $(BIN)103.x

$(BIN)101.x: $(OBJ)101.o $(RANDOM)$(OBJ)random.o $(GENALG)$(OBJ)individual.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)101.o: $(SRC)101.cpp $(wildcard $(INC)*.h) $(wildcard $(CFG)10*) $(RANDOM)$(INC)random.h $(wildcard $(GENALG)$(INC)*.h) | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/101.cpp -o $@

$(BIN)102.x: $(OBJ)102.o $(RANDOM)$(OBJ)random.o $(GENALG)$(OBJ)individual.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)102.o: $(SRC)102.cpp $(wildcard $(INC)*.h) $(wildcard $(CFG)10*) $(RANDOM)$(INC)random.h $(wildcard $(GENALG)$(INC)*.h) | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/102.cpp -o $@

$(BIN)103.x: $(OBJ)103.o $(RANDOM)$(OBJ)random.o $(GENALG)$(OBJ)individual.o $(MISCFC)$(OBJ)misc.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
$(OBJ)103.o: $(SRC)103.cpp $(wildcard $(INC)*.h) $(wildcard $(CFG)10*) $(RANDOM)$(INC)random.h $(wildcard $(GENALG)$(INC)*.h) | $(OBJ) 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/103.cpp -o $@

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
