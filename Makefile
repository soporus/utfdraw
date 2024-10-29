all: main
CC = clang
override CFLAGS += -std=gnu23 -o3 -s -Wconversion -Wno-sign-conversion -Wdouble-promotion -flto=full -march=native -Wpedantic -DTB_LIB_OPTS=1 -DTB_OPT_ATTR_W=32 -DTB_OPT_PRINTF_BUF=100
GCC = gcc
override GCCFLAGS += -std=gnu23 -o3 -s -Wconversion -Wno-sign-conversion -Wdouble-promotion -flto -march=native -Wpedantic -DTB_LIB_OPTS=1 -DTB_OPT_ATTR_W=32 -DTB_OPT_PRINTF_BUF=100 -fanalyzer 

#export ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0
export ASAN_OPTIONS=abort_on_error=0:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1

export G_SLICE=always-malloc G_DEBUG=gc-friendly

LDFLAGS = $(CFLAGS) -flto=full

GOLDFLAGS = $(GCCFLAGS) -flto

DBLD_FLAGS = -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -flto

#SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
#HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)
SRCS = main.c term.c
HEADERS = term.h termbox2/termbox2.h
main: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o  "$@"
main-debug: $(SRCS) $(HEADERS)
	$(CC) -std=c2x -Og -ggdb3 -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -fanalyzer $(SRCS) $(DBLD_FLAGS) -o "$@"
main-gcc: $(SRCS) $(HEADERS)
	$(GCC) $(GCCFLAGS) $(SRCS) $(GOLDFLAGS) -o main-gcc

strip: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o  main && strip --strip-all --verbose main

clean:
	rm -f main main-debug
