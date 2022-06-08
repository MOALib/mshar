CSTANDARD=c90
CFLAGS=-g -Og -ansi -std=$(CSTANDARD) -Wall -Werror -Wextra -Wpedantic -pedantic -pedantic-errors -fdiagnostics-color -pipe
CSHARED=-shared -fPIC -Wl,-soname,libmshar.so

CC=gcc
MINGWCC=i686-w64-mingw32-$(CC)
MINGW64CC=x86_64-w64-mingw32-$(CC)

MSHARC=mshar.c
MSHARLIB=msharlib.c

MSHAR_BIN=./mshar.exe
LIBMSHAR_NOEND=./libmshar
LIBMSHAR_WIN=$(LIBMSHAR_NOEND).dll
LIBMSHAR=$(LIBMSHAR_NOEND).so
LIBMSHAR_WIN64=$(LIBMSHAR_NOEND)64.dll

BIND_DIR=./binding
PYBIND_DIR=$(BIND_DIR)/pymshar
CSBIND_DIR=$(BIND_DIR)/msharsharp

.DEFAULT_GOAL := all

.PHONY: all cls   build app windllso dllso binding   docs   clean execclean docclean  init

all: cls build

cls:
	@clear || cls



build: app windllso dllso

app:
	$(CC) $(MSHARC) $(CFLAGS) -o $(MSHAR_BIN)

windllso:
	$(MINGWCC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR_WIN)
	$(MINGW64CC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR_WIN64)

#	cp $(LIBMSHAR_WIN) $(CSBIND_DIR)/$(LIBMSHAR_WIN)
#	cp $(LIBMSHAR_WIN) $(PYBIND_DIR)
#	cp $(LIBMSHAR_WIN64) $(PYBIND_DIR)

dllso:
	$(CC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR)
#	cp $(LIBMSHAR) $(PYBIND_DIR)
#	cp $(LIBMSHAR) $(CSBIND_DIR)/$(LIBMSHAR_NOSO)

binding:
	@echo "Delegating to $(BIND_DIR) makefile"
	@cd $(BIND_DIR) && make





docs: cls
	doxygen Doxyfile

clean: execclean docclean

execclean: cls
	@echo "Cleaning"

	@echo "Cleaning shared libraries"
	@-rm $(LIBMSHAR) $(LIBMSHAR_WIN) $(LIBMSHAR_WIN64) 2> /dev/null || true

	@echo "cleaning binaries"
	@-rm $(MSHAR_BIN) 2> /dev/null || true

	@echo "Delegating to $(BIND_DIR) makefile"
	@cd $(BIND_DIR) && make clean
#	rm $(PYBIND_DIR)/$(LIBMSHAR) 
#	rm $(CSBIND_DIR)/$(LIBMSHAR_NOSO) 
#	rm $(PYBIND_DIR)/$(LIBMSHAR_WIN) 
#	rm $(CSBIND_DIR)/$(LIBMSHAR_WIN) 
#	rm $(PYBIND_DIR)/$(LIBMSHAR_WIN64)

docclean:
	@echo "Clobbering docs"
	@-rm -rf html 2> /dev/null || true
	@-rm -rf htmldoc 2> /dev/null || true







init: cls
	@echo "Initializing"

	@echo "Delegating to $(BIND_DIR) makefile"
	@cd $(BIND_DIR) && make init