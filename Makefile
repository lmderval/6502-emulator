CC = g++
CPPFLAGS = -Iinclude
CFLAGS = -std=c++20 -Wall -Wextra -Werror -Wvla -pedantic

BIN = main
OBJ = src/$(BIN).o \
	  src/memory.o \
	  src/processor.o \
	  src/program.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^

clean:
	$(RM) $(BIN) $(OBJ)

.PHONY: all clean
