ifeq ($(OS),Windows_NT)
	OUTEXT := .exe
endif

OUT ?= bin/alpha-calc$(OUTEXT)

CFLAGS ?= -s -ansi -std=c89 -Ofast -Os -Iinclude -lm

all: $(OUT)

$(OUT): src/alpha-calc.c
	$(CC) -m32 $(CFLAGS) src/alpha-calc.c -o $@

.PHONY: clean
clean:
	$(RM) $(OUT)
