CXX = g++
CXXFLAGS = -Wall -O3 --std=c++11

.PHONY: clean

OBJ = obj/
SRC = src/
INC = include/

TARGET = misc

$(OBJ)$(TARGET).o: $(SRC)$(TARGET).cpp $(INC)$(TARGET).h | $(OBJ)  
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ):
	@echo "Creating $@"
	@mkdir -p $@

clean:
	@rm -rf $(OBJ)
	@echo "All clean"
