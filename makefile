MAIN=./mshar

TEXIDOC=./docs/mshar.texi
TEXIOUT=./docs/mshar.info

SHDOC=./modules/shdoc/shdoc
SHDOC_OUT=./docs/shdoc.md

.PHONY: all docs viewdocs

all: docs

docs:
	makeinfo $(TEXIDOC) -o $(TEXIOUT)
	makeinfo $(TEXIDOC) --html
	$(SHDOC) < $(MAIN) > $(SHDOC_OUT)

viewdocs:
	info -f $(TEXIOUT)