CC = g++
CFLAGS = -Wall -Wextra -Wpedantic
LIBS = -lfmt
INPUTS = src/test.cpp
OUTPUT = test

.PHONY: all

all:
	$(CC) $(CFLAGS) $(LIBS) -o $(OUTPUT) $(INPUTS)

clean:
	rm $(OUTPUT)
