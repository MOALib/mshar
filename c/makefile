CFLAGS=-g -Og -ansi -Wall -Werror -Wextra -pedantic -pedantic-errors -fdiagnostics-color -pipe
CC=gcc

.PHONY: all cls build docs

all: cls build

cls:
	@clear || cls

build:
	$(CC) mshar.c $(CFLAGS) -o ./mshar.exe

docs: cls
	doxygen Doxyfile