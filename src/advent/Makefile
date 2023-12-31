ROOT = $(realpath .)
IDP_DEV_ROOT = $(realpath ../..)/idp-udev
INC_DIR = $(IDP_DEV_ROOT)/include
UNC_DIR_CPM = $(IDP_DEV_ROOT)/src/ulibc
BUILD_DIR = $(ROOT)/build
BIN_DIR = $(ROOT)/bin
BUILD_DIR_SDK = $(IDP_DEV_ROOT)/build
AUX_DIR = $(realpath ../..)/make-aux

CC = sdcc
CFLAGS = --std-c11 -mz80 -I. -I$(INC_DIR) -I$(UNC_DIR_CPM) --nostdinc

LD = sdcc
LDFLAGS = -mz80 -Wl -y --code-loc 0x100 --no-std-crt0 --nostdlib --nostdinc -L$(BUILD_DIR_SDK) -lulibc -lusdcc -p

L2 = sdldz80
L2FLAGS = -nf
L2FIX = sed '/-b _DATA = 0x8000/d'

TARGET = advent

SRC = $(wildcard *.c)
BIN_SRC = $(wildcard *.bin)
BIN_DST = $(addprefix $(BIN_DIR)/,$(BIN_SRC))
OBJ = $(patsubst %.c,$(BUILD_DIR)/%.rel,$(SRC))
HEX = $(BUILD_DIR)/$(TARGET).ihx
COM = $(BIN_DIR)/$(TARGET).com
IMG = $(BIN_DIR)/$(TARGET).img
HFE = $(BIN_DIR)/$(TARGET).hfe

CRT0 = crt0

.PHONY: all
all: com hfe

.PHONY: obj
obj: $(OBJ)

.PHONY: hex
hex: $(HEX)

.PHONY: bin com
bin com: $(COM) $(BIN_DST)
	@# Remove obsolete BIN files
	@for data_file in $(wildcard $(BIN_DIR)/*.bin) ; do \
		if [ ! -f $$(basename $$data_file) ] ; then \
			echo rm -f $$data_file ; \
			rm -f $$data_file ; \
		fi \
	done

.PHONY: img
img: $(IMG)

.PHONY: hfe
hfe: $(HFE)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# $(BIN_DST) ; Required for 'bin'/'com'
$(BIN_DIR)/%.bin: %.bin | $(BIN_DIR)
	cp $< $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# $(OBJ) ; Additional header dependencies below
$(BUILD_DIR)/%.rel: %.c %.h | $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(HEX): $(OBJ)
	$(LD) $(LDFLAGS) -o $(HEX) $(BUILD_DIR_SDK)/$(CRT0).rel $(OBJ)
	$(L2FIX) $(BUILD_DIR)/$(TARGET).lk > $(BUILD_DIR)/$(TARGET).link
	$(L2) $(L2FLAGS) $(BUILD_DIR)/$(TARGET).link

$(COM): $(HEX) | $(BIN_DIR)
	sdobjcopy -I ihex -O binary $(HEX) $(COM)

$(IMG): $(COM) $(BIN_SRC)
	cp $(AUX_DIR)/bootg.img $(IMG)
	@echo cpmcp -f idpfdd $(IMG) $(COM) 0:$(TARGET).com
	@(cd $(AUX_DIR) ; cpmcp -f idpfdd $(IMG) $(COM) 0:$(TARGET).com)
	@for data_file in $(BIN_SRC) ; do \
		echo cpmcp -f idpfdd $(IMG) $(ROOT)/$$data_file 0:$$data_file ; \
		(cd $(AUX_DIR) ; cpmcp -f idpfdd $(IMG) $(ROOT)/$$data_file 0:$$data_file) ; \
	done

$(HFE): $(IMG)
	-hxcfe -uselayout:IDP -conv:HXC_HFE -finput:$(IMG) -foutput:$(HFE)

$(BUILD_DIR)/wizard.rel: hdr.h save.h utils.h
$(BUILD_DIR)/vocab.rel: hdr.h utils.h io.h
$(BUILD_DIR)/utils.rel: hdr.h vocab.h
$(BUILD_DIR)/trav.rel: hdr.h utils.h io.h
$(BUILD_DIR)/subr.rel: hdr.h io.h wizard.h vocab.h trav.h utils.h
$(BUILD_DIR)/save.rel: hdr.h utils.h vocab.h trav.h
$(BUILD_DIR)/io.rel: hdr.h subr.h utils.h
$(BUILD_DIR)/hdr.rel: trav.h
$(BUILD_DIR)/done.rel: hdr.h io.h subr.h vocab.h utils.h
$(BUILD_DIR)/advent.rel: hdr.h _init.h vocab.h io.h done.h subr.h wizard.h utils.h save.h
$(BUILD_DIR)/_init.rel: hdr.h trav.h io.h vocab.h wizard.h utils.h