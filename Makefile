ifeq ($(OS),Windows_NT)
	OUTEXT := .exe
endif

MODERN_OUT ?= bin/Modern/alpha-calc$(OUTEXT)
C64_OUT ?= bin/C64/alpha-calc.prg

ifndef (C64_CC)
	C64_CC = cl65
endif

CFLAGS ?= -s -ansi -std=c89 -Ofast -Os -lm

all: $(MODERN_OUT) $(C64_OUT)

$(MODERN_OUT): alpha-calc.c
	$(CC) $(CFLAGS) $< -o $@

$(C64_OUT): alpha-calc.c
	$(C64_CC) $< -o$@ -t c64

.PHONY: clean
clean:
	$(RM) $(MODERN_OUT) $(C64_OUT)
