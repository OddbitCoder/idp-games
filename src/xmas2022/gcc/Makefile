ROOT = $(realpath .)
BUILD_DIR = $(ROOT)/build
BIN_DIR = $(ROOT)/bin

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))

.PHONY: all
all: $(BIN_DIR)/xmas

$(BIN_DIR)/xmas: $(OBJ) | $(BIN_DIR)
	gcc $(OBJ) -o $(BIN_DIR)/xmas2022

$(BUILD_DIR)/%.o: %.c %.h | $(BUILD_DIR)
	gcc -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(BUILD_DIR)/*
