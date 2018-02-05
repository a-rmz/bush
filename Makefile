CC = gcc
FLAGS = -c

SRC = src
BUILD = bin

INIT = init
GETTY = getty
SH = sh

# SOURCES = $(shell find . -name *.c)
# OBJECTS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SOURCES))

all: dir getty init sh

init: $(SRC)/init.c
	$(CC) $^ -o $(BUILD)/$(INIT)

sh: $(SRC)/sh.c $(SRC)/utils/io.c
	$(CC) $^ -o $(BUILD)/$(SH)

getty: $(SRC)/getty.c $(SRC)/utils/io.c
	$(CC) $^ -o $(BUILD)/$(GETTY)

dir:
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)/*
