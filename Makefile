## kmetro - kluzey's metronome

BUILD_DIR = ./build
SRC_DIR = ./src
INSTALL_PATH = /usr/local/bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/audio.c $(SRC_DIR)/beat.c $(SRC_DIR)/util.c
OBJ = $(subst $(SRC_DIR),$(BUILD_DIR),$(SRC:.c=.o))
BIN = kmetro

CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lopenal -lalut

all: $(BIN) options

options:
	@echo kmetro build options:
	@echo "CC 	= $(CC)"
	@echo "CFLAGS	= $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) 

clean:
	rm -rf $(BIN) $(BUILD_DIR)

install:
	mv $(BIN) $(INSTALL_PATH)

.PHONY: clean all
