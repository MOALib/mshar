MSHAR_SWIG=mshar.swigi
MSHAR_H=../src/mshar.h
thisSrc=src

CSBIND_DIR=msharsharp
PYBIND_DIR=pymshar
JAVABIND_DIR=mshar4j

.DEFAULT_GOAL := all

.PHONY: all   csharp python  clean init

all: csharp python

csharp:
	@echo "Generating C# Bindings"
	@cp $(MSHAR_SWIG) $(CSBIND_DIR)
	@cd $(CSBIND_DIR) && mkdir $(CSBIND_DIR) 2> /dev/null || true
	@cd $(CSBIND_DIR) && swig -csharp -namespace MXPSQL.MSharSharp -outdir $(CSBIND_DIR) $(MSHAR_SWIG)

python:
	@echo "Generating Python Bindings"
	@cp $(MSHAR_SWIG) $(PYBIND_DIR)
	@cd $(PYBIND_DIR) && swig -python -outdir . $(MSHAR_SWIG)


clean:
	@echo "Cleaning C# binding"
	@-rm -rf $(CSBIND_DIR)/* 2> /dev/null || true

	@echo "Cleaning Python binding"
	@-rm -rf $(PYBIND_DIR)/* 2> /dev/null || true

	@echo "Removing copied source directory"
	@-rm -rf $(thisSrc) 2> /dev/null || true

init:
	@echo "Initializing"

	@echo "Making source directory and copying source file"
	@-mkdir $(thisSrc) 2> /dev/null || true
	@-cp $(MSHAR_H) $(thisSrc)

	@echo "Making C# bindings directory"
	@-mkdir $(CSBIND_DIR) 2> /dev/null || true

	@echo "Making Python bindings directory"
	@-mkdir $(PYBIND_DIR) 2> /dev/null || true