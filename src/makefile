CC = gcc
BIN_DIR = ../build
OBJ_DIR = $(BIN_DIR)/obj
TEST_DIR = test

CCFLAGS = -std=c99 -g -Wall
INCLUDES = -I.
SRC_FILES = ubigint.c 
VPATH = $(SRC_FILES) $(TEST_DIR)

all: basic_test

basic_test: $(SRC_FILES) basic.c
	$(CC) $(CCFLAGS) $(INCLUDES) $^ -o $(BIN_DIR)/$@

.c.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $<  -o $@

# DO NOT DELETE THIS LINE -- make depend needs it
