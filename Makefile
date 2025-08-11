CC ?= gcc

# all C files in /src
SRC = $(wildcard src/*.c) $(wildcard src/detectors/*.c) $(wildcard src/detectors/god_class/*.c)

# third party paths
TS_DIR = third_party/tree-sitter
TS_LIB = $(TS_DIR)/libtree-sitter.a
TS_INC = -I$(TS_DIR)/lib/include
GRAMMAR = third_party/tree-sitter-matlab/src/parser.c third_party/tree-sitter-matlab/src/scanner.c
GRAMMAR_INC = -Ithird_party/tree-sitter-matlab/src
TINYDIR_INC = -Ithird_party

PROJECT_INC = -Isrc -Isrc/detectors -Isrc/detectors/god_class

BIN = main

CFLAGS = -std=c11 -O2 $(TS_INC) $(TINYDIR_INC) $(GRAMMAR_INC) $(PROJECT_INC)
LDFLAGS = $(TS_LIB)

.PHONY: all
all: $(BIN)

# clone third party repositories using git
.PHONY: setup
setup:
	cd third_party && git clone https://github.com/tree-sitter/tree-sitter.git
	cd third_party && git clone https://github.com/acristoffers/tree-sitter-matlab.git

$(BIN): $(SRC) $(GRAMMAR) $(TS_LIB)
	$(CC) $(CFLAGS) -o $(BIN) $^ $(LDFLAGS)

# build libtree-sitter.a by calling make in TS_DIR
$(TS_LIB):
	$(MAKE) -C $(TS_DIR)

.PHONY: clean
clean:
	rm -f $(BIN)

.PHONY: clean-all
clean-all: clean
	rm -rf third_party/tree-sitter/
	rm -rf third_party/tree-sitter-matlab/
