CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11

.PHONY: clean

BIN := bin/
OBJ := obj/
SRC := src/
INC := include/
LIB := ../lib/

RANDOM := $(LIB)Random/
GENALG := $(LIB)Ga/


TARGET := 091 
$(BIN)091.x: $(OBJ)091.o $(RANDOM)$(OBJ)random.o $(GENALG)$(OBJ)individual.o | $(BIN)
	@echo "Linking $^"
	$(CXX) $(CXXFLAGS) $^ -o $@
  # $(wildcard $(INC)*.h)
obj/091.o: src/091.cpp include/city.h include/crossovers.h include/mutations.h $(RANDOM)$(INC)random.h | obj/ 
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c src/091.cpp -o $@

$(GENALG)$(OBJ)individual.o: 
	$(MAKE) -C $(GENALG)

$(RANDOM)$(OBJ)random.o: 
	$(MAKE) -C $(RANDOM)


$(BIN):
	@echo "Creating $@"
	@mkdir -p $@

$(OBJ):
	@echo "Creating $@"
	@mkdir -p $@

clean:
	@rm -rf $(OBJ) $(BIN)
	@echo "All clean"
