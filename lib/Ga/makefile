CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11

.PHONY: clean

OBJ = obj/
SRC = src/
INC = include/

RANDOM = ../Random/

TARGET = individual
$(OBJ)$(TARGET).o: $(SRC)$(TARGET).cpp $(wildcard $(INC)*.h) $(RANDOM)$(INC)random.h | $(OBJ)  
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ):
	@echo "Creating $@"
	@mkdir -p $@

clean:
	@rm -rf $(OBJ)
	@echo "All clean"
