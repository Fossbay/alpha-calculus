ifeq ($(OS),Windows_NT)
	OUTEXT := .exe
endif

MODERN_OUT ?= bin/Modern/alpha-calc$(OUTEXT)
APPLE2_OUT ?= bin/AppleII/alpha-calc.apple2
C64_OUT ?= bin/C64/alpha-calc.prg
C128_OUT ?= bin/C128/alpha-calc.prg
PLUS4_OUT ?= bin/Plus4/alpha-calc.prg
X16_OUT ?= bin/X16/alpha-calc.prg

ifndef (6502_CC)
	6502_CC = cl65
endif

CFLAGS ?= -s -ansi -std=c89 -Ofast -Os -lm

all: $(MODERN_OUT) $(C64_OUT) $(C128_OUT) $(PLUS4_OUT) $(X16_OUT)

$(MODERN_OUT): alpha-calc.c
	$(CC) $(CFLAGS) $< -o $@

$(C64_OUT): alpha-calc.c
	$(6502_CC) $< -o$@ -t c64

$(C128_OUT): alpha-calc.c
	$(6502_CC) $< -o$@ -t c128

$(PLUS4_OUT): alpha-calc.c
	$(6502_CC) $< -o$@ -t plus4

$(X16_OUT): alpha-calc.c
	$(6502_CC) $< -o$@ -t cx16

.PHONY: clean
clean:
	$(RM) $(MODERN_OUT) $(C64_OUT) $(C128_OUT) $(PLUS4_OUT) $(X16_OUT)
