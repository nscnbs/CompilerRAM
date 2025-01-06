CXX := g++
CXXFLAGS := -std=c++17 -pedantic
INCLUDES := -I ../include

SRC_DIR := ./src
OBJ_DIR := ./src
SRC_FILES := utils.cpp lexer.cpp parser.cpp main.cpp
OBJ_FILES := $(SRC_FILES:.cpp=.o)

BISON_FILE := parser.y
FLEX_FILE := lexer.l
BISON_SRC := $(SRC_DIR)/parser.cpp
BISON_HEADER := $(SRC_DIR)/parser.hpp
FLEX_SRC := $(SRC_DIR)/lexer.cpp

TARGET := kompilator

all: $(TARGET)

$(TARGET): $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))
	$(CXX) $(INCLUDES) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BISON_SRC) $(BISON_HEADER): $(SRC_DIR)/$(BISON_FILE)
	bison -d -o $(BISON_SRC) $<

$(FLEX_SRC): $(SRC_DIR)/$(FLEX_FILE) $(BISON_HEADER)
	flex -o $@ $<

.PHONY: clean

clean:
	rm -f $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/parser.hpp $(SRC_DIR)/*.o $(EXECUTABLE)
