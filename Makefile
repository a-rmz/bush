
CC = gcc
FLAGS = -c

SRC = src
BUILD = build

INIT = init
GETTY = getty
SH = sh

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SOURCES))

all: init getty sh

init: dir $(BUILD)/$(INIT)

getty: dir $(BUILD)/$(GETTY)

sh: dir $(BUILD)/$(SH)

dir:
	mkdir -p $(BUILD)

$(BUILD)/$(EXE): $(OBJECTS)
	$(CC) $^ -o $@

$(OBJECTS): $(BUILD)/%.o : $(SRC)/%.c
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f $(BUILD)/*.o $(BUILD)/$(INIT)
	rm -f $(BUILD)/*.o $(BUILD)/$(GETTY)
	rm -f $(BUILD)/*.o $(BUILD)/$(SH)
