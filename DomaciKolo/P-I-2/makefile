CXX=g++
CFLAGS=-g -pipe -static -Wall -std=c++20 -Isrc
LDFLAGS=

SRC=src
OBJ=obj
SRCS=$(shell find $(SRC) -name '*.cpp')
OBJS=$(patsubst $(SRC)%.cpp, $(OBJ)/%.o, $(SRCS))
HDRS=$(shell find $(SRC) -name '*.h')
BIN=bin/main

all: $(BIN)

$(BIN): $(OBJS) $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $@

clean:
	rm -rf $(OBJ)
	rm -f $(BIN)