CC = gcc
CC_OPTS = -o
SRC = main.c
BIN = mkdoc
INSTALLATION = /usr/bin/

$(BIN): $(SRC)
	$(CC) $(SRC) $(CC_OPTS) $(BIN) 

install: $(BIN)
	cp $(BIN) $(INSTALLATION)
.PHONY: install

uninstall:
	rm $(INSTALLATION)$(BIN)
.PHONY: uninstall

clean:
	rm $(BIN)
.PHONY: clean
