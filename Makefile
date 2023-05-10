MAKEFLAGS += -rR

.EXTRA_PREREQS:= $(abspath $(lastword $(MAKEFILE_LIST)))

PROGRAM_NAME=lithp
P=$(PROGRAM_NAME)

INCLUDE_DIR=inc
OBJECT_DIR=obj
BUILD_DIR=build
SOURCE_DIR=src

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -O3 -D_FORTIFY_SOURCE=2 -I$(INCLUDE_DIR) -I$(OBJECT_DIR) # OBJECT_DIR contains the precompiled headers
LDFLAGS=
DBGFLAGS=-g3
SANFLAGS=-fsanitize=address,undefined,integer-divide-by-zero -fno-omit-frame-pointer # Flags for ASan builds

LDLIBS=-lreadline -lm

# Header files
HEADERS=parser mpc
# Compilation units
UNITS=lithp parser mpc

# Generate lists of headers, sources, objects, and object-with-debugging-symbols
HDRS=$(patsubst %, $(OBJECT_DIR)/%.h.gch, $(HEADERS))
SRCS=$(patsubst %,$(SOURCE_DIR)/%.c, $(UNITS))
OBJS=$(patsubst %, $(OBJECT_DIR)/%.o, $(UNITS))
OBJS_DEBUG=$(patsubst %,$(OBJECT_DIR)/%.o.debug, $(UNITS))

# build main binary by default
.PHONY: default
default: $(BUILD_DIR)/$(P)

# build main binary, main binary with debug symbols, and main binary built for valgrind
.PHONY: all
all: $(BUILD_DIR)/$(P) $(BUILD_DIR)/$(P)_debug $(BUILD_DIR)/$(P)_sanitize

# Build pre-compiled header file
.SECONDARY: $(OBJECT_DIR)/%.gch
$(OBJECT_DIR)/%.gch: $(INCLUDE_DIR)/%
	@echo Building pre-compiled header: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# Build object file
.SECONDARY: $(OBJECT_DIR)/%.o
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HDRS)
	@echo Building object: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -c -o $@ $(LDLIBS)

# Build object file with debugging symbols
.SECONDARY: $(OBJECT_DIR)/%.o.debug
$(OBJECT_DIR)/%.o.debug: $(SOURCE_DIR)/%.c $(HDRS)
	@echo Building object with debug symbols: $@
	@mkdir -p $(OBJECT_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $< -c -o $@ $(LDLIBS)

# Build main binary
$(BUILD_DIR)/$(P): $(OBJS)
	@echo Building $@
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/$(P)_sanitize: $(OBJS_DEBUG)
	@echo Building $@
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $(SANFLAGS) $^ -o $@ $(LDLIBS)

# Build binary with debugging symbols.
$(BUILD_DIR)/$(P)_debug: $(OBJS_DEBUG)
	@echo Building $@
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBGFLAGS) $^ -o $@ $(LDLIBS)

# Run program
.PHONY: run
run: $(BUILD_DIR)/$(P)
	$^

# Start gdb on debug build
.PHONY: debug
debug: $(BUILD_DIR)/$(P)_debug
	gdb $^

# Run valgrind on debug build
.PHONY: grind
grind: $(BUILD_DIR)/$(P)_debug
	valgrind --track-origins=yes --leak-check=full --leak-resolution=high $^

# Run linter
.PHONY: lint
lint: $(SRCS)
	clang-tidy\
	 --quiet\
	 --checks=*,\
	-llvmlibc-restrict-system-libc-headers,\
	-altera-id-dependent-backward-branch,\
	-modernize-macro-to-enum,\
	-altera-unroll-loops,\
	-llvm-include-order,\
	-bugprone-reserved-identifier,\
	-cert-dcl37-c,\
	-cert-dcl51-cpp,\
	-misc-no-recursion,\
	-google-readability-todo\
	 $^\
	 -- -I$(INCLUDE_DIR) # Have to include this or clang will complain about not being able to find header files. See https://stackoverflow.com/a/56457021/15446749

# Format all sources
.PHONY: format
format: $(SRCS)
	clang-format --fallback-style=LLVM -i $(SRCS)

.PHONY: clean
clean:
	rm -rf $(OBJECT_DIR)
	rm -rf $(BUILD_DIR)
