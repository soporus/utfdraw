all: main

CC = gcc
override CFLAGS += -std=gnu17 -o0 -Wconversion -Wno-sign-conversion -Wdouble-promotion -ffunction-sections --warn-function-elimination -flto -v -march=native -fwhole-program

LDGFLAGS = $(CFLAGS)

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o  "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CC) -std=gnu17 -fvar-tracking -fsanitize=address -ggdb3 -v $(SRCS) -o "$@"

strip: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o  main && strip --strip-all --verbose main

clean:
	rm -f main main-debug
