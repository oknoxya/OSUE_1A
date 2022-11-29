CC = gcc

# newer versions of gcc use _DEFAULT_SOURCE instead of _BSD_SOURCE and/or _SVID_SOURCE
DEFS = -std=c99 -pedantic -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L -g
CFLAGS = -Wall $(DEFS)

TARGET = mydiff

SRC = $(PWD)/src
OUT = $(PWD)
BUILD = $(PWD)/build

SRCSFILES = main.c cli.c diff.c
SRCS = $(addprefix $(SRC)/,$(SRCSFILES))
OBJS = $(addprefix $(BUILD)/,$(SRCSFILES:%.c=%.o))

all: folders $(TARGET)

clean:
	rm -rf $(BUILD)/* $(OUT)/$(TARGET) $(OUT)/$(TARGET).tgz

folders:
	mkdir -p $(BUILD)
	mkdir -p $(OUT)

package:
	rm -f $(OUT)/$(TARGET).tgz
	tar -cvzf $(OUT)/$(TARGET).tgz -C $(PWD) src/* Makefile

.PHONY: all clean folders package

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c 
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/main.o: $(SRC)/main.c $(SRC)/cli.h $(SRC)/diff.h
$(BUILD)/cli.o: $(SRC)/cli.c $(SRC)/cli.h
$(BUILD)/diff.o: $(SRC)/diff.c $(SRC)/diff.h