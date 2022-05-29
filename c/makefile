CFLAGS=-g -Og -ansi -std=c90 -Wall -Werror -Wextra -Wpedantic -pedantic -pedantic-errors -fdiagnostics-color -pipe
CSHARED=-shared -fPIC -Wl,-soname,libmshar.so
CC=gcc
MINGWCC=i686-w64-mingw32-$(CC)
MINGW64CC=x86_64-w64-mingw32-$(CC)

MSHARC=mshar.c
MSHARLIB=msharlib.c

MSHAR_BIN=./mshar.exe
LIBMSHAR_WIN=./libmshar.dll
LIBMSHAR_WIN64=./libmshar64.dll
LIBMSHAR_NOSO=./libmshar
LIBMSHAR=$(LIBMSHAR_NOSO).so

BIND_DIR=./binding
PYBIND_DIR=./binding/pymshar
CSBIND_DIR=./binding/msharsharp/msharsharp

.PHONY: all cls build app windllso dllso docs clean

all: cls build

cls:
	@clear || cls



build: app windllso dllso

app:
	$(CC) $(MSHARC) $(CFLAGS) -o $(MSHAR_BIN)

windllso:
	$(MINGWCC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR_WIN)
	$(MINGW64CC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR_WIN64)

	cp $(LIBMSHAR_WIN) $(CSBIND_DIR)/$(LIBMSHAR_WIN)
	cp $(LIBMSHAR_WIN) $(PYBIND_DIR)
	cp $(LIBMSHAR_WIN64) $(PYBIND_DIR)

dllso:
	$(CC) $(MSHARLIB) $(CFLAGS) $(CSHARED) -o $(LIBMSHAR)
	cp $(LIBMSHAR) $(PYBIND_DIR)
	cp $(LIBMSHAR) $(CSBIND_DIR)/$(LIBMSHAR_NOSO)





docs: cls
	doxygen Doxyfile



clean:
	rm $(LIBMSHAR) $(LIBMSHAR_WIN) $(LIBMSHARR_WIN64)
	rm $(PYBIND_DIR)/$(LIBMSHAR) 
	rm $(CSBIND_DIR)/$(LIBMSHAR_NOSO) 
	rm $(PYBIND_DIR)/$(LIBMSHAR_WIN) 
	rm $(CSBIND_DIR)/$(LIBMSHAR_WIN) 
	rm $(PYBIND_DIR)/$(LIBMSHAR_WIN64)