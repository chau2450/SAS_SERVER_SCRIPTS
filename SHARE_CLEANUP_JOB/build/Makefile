CC=gcc
CFLAGS=-Wall -Werror -static
SRC_DIR=../src
OBJ_DIR=../obj
BIN_DIR=../bin

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(OBJ_DIR)/main.o $(OBJ_DIR)/list.o $(OBJ_DIR)/traverse_dirs.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/list.o: $(SRC_DIR)/list.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/traverse_dirs.o: $(SRC_DIR)/traverse_dirs.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
