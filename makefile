MAIN=./mshar

TEXIDOC=./docs/mshar.texi
TEXIOUT=./docs/mshar.info

SHDOC=./modules/shdoc/shdoc
SHDOC_OUT=./docs/shdoc.md

.PHONY: all docs

all: docs

docs:
	makeinfo $(TEXIDOC) -o $(TEXIOUT)
	$(SHDOC) < $(MAIN) > $(SHDOC_OUT)