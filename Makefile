CXX      := g++
CXXFLAGS := -W -pedantic -std=c++17

SRC_DIR  := ./src
OBJ_DIR  := $(SRC_DIR)

SRC_FILES := lexer.cpp parser.tab.cpp main.cpp manager.cpp instructions.cpp expressions.cpp conditions.cpp
OBJ_FILES := $(SRC_FILES:%.cpp=$(OBJ_DIR)/%.o)

BISON_FILE   := parser.y
FLEX_FILE    := lexer.l
BISON_SRC    := $(SRC_DIR)/parser.tab.cpp
BISON_HDR    := $(SRC_DIR)/parser.tab.hpp
FLEX_SRC     := $(SRC_DIR)/lexer.cpp

TARGET := kompilator

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BISON_SRC) $(BISON_HDR): $(SRC_DIR)/$(BISON_FILE)
	bison -d -o $(BISON_SRC) $<

$(FLEX_SRC): $(SRC_DIR)/$(FLEX_FILE) $(BISON_HDR)
	flex -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET) $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.tab.* $(SRC_DIR)/*.o $(SRC_DIR)/*.log $(EXECUTABLE)
