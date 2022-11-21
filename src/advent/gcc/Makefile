ROOT = $(realpath .)
BUILD_DIR = $(ROOT)/build
BIN_DIR = $(ROOT)/bin

all: $(BIN_DIR)/advent

$(BIN_DIR)/advent: $(BUILD_DIR)/advent.o | $(BIN_DIR)
	gcc -o $@ $<

$(BUILD_DIR)/%.o: %.c %.h | $(BUILD_DIR)
	gcc -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*
	rm -f $(BUILD_DIR)/*

$(BUILD_DIR)/wizard.o: hdr.h save.h utils.h
$(BUILD_DIR)/vocab.o: hdr.h utils.h io.h
$(BUILD_DIR)/utils.o: hdr.h vocab.h
$(BUILD_DIR)/trav.o: hdr.h utils.h io.h
$(BUILD_DIR)/subr.o: hdr.h io.h wizard.h vocab.h trav.h utils.h
$(BUILD_DIR)/save.o: hdr.h utils.h vocab.h trav.h
$(BUILD_DIR)/io.o: hdr.h subr.h utils.h
$(BUILD_DIR)/hdr.o: trav.h
$(BUILD_DIR)/done.o: hdr.h io.h subr.h vocab.h utils.h
$(BUILD_DIR)/advent.o: hdr.h _init.h vocab.h io.h done.h subr.h wizard.h utils.h save.h
$(BUILD_DIR)/_init.o: hdr.h trav.h io.h vocab.h wizard.h utils.h