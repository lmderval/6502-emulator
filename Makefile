CC = g++
CPPFLAGS = -Iinclude
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -Wvla -pedantic

BIN = main
OBJ = src/$(BIN).o \
	  src/memory.o \
	  src/processor.o \
	  src/program.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

debug: CXXFLAGS += -g -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: $(BIN)

clean:
	$(RM) $(BIN) $(OBJ)

.PHONY: all debug clean
