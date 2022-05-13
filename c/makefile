.PHONY: all cls build docs

all: cls build

cls:
	@clear || cls

build:
	gcc mshar.c -g -Og -ansi -Wall -pedantic -fdiagnostics-color -o ./mshar.exe

docs: cls
	doxygen Doxyfile