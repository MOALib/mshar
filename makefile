MAIN_FILE=mshar
MAIN=./$(MAIN_FILE)

INSTALL_DIR=/usr/local/bin
INSTALL_PATH=$(INSTALL_DIR)/$(MAIN_FILE)

TEXIDOC=./docs/mshar.texi
TEXIOUT=./docs/mshar.info

SHDOC=./modules/shdoc/shdoc
SHDOC_OUT=./docs/shdoc.md

.PHONY: all cls list noop install uninstall docs viewdocs

all: cls docs

noop:
	@;

cls:
	clear || cls;

# Target: list
list:
	@grep "# Target: *" makefile | sed '/grep/d' | tr -d '# ' | sed 's/[][*]\|[[:space:]]//' | sed 's/Target://g'


# Target: docs
docs:
	makeinfo $(TEXIDOC) -o $(TEXIOUT)
	makeinfo $(TEXIDOC) --html
	$(SHDOC) < $(MAIN) > $(SHDOC_OUT)

# Target: install
install:
	cp "$(MAIN)" "$(INSTALL_PATH)"

# Target: uninstall
uninstall:
	rm "$(INSTALL_PATH)"

# Target: viewdocs
viewdocs:
	info -f $(TEXIOUT)