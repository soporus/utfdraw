all: main
CC = gcc
override CFLAGS += -std=gnu17 -oS -Wconversion -Wno-sign-conversion -Wdouble-promotion -ffunction-sections -flto -v -march=native -Wpedantic -fanalyzer

#export ASAN_OPTIONS=strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0
export ASAN_OPTIONS=abort_on_error=0:fast_unwind_on_malloc=0:detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1

export G_SLICE=always-malloc G_DEBUG=gc-friendly

LDFLAGS = $(CFLAGS)

DBLD_FLAGS = -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -flto -v

SRCS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.c' -print)
HEADERS = $(shell find . -name '.ccls-cache' -type d -prune -o -type f -name '*.h' -print)

main: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o  "$@"
main-debug: $(SRCS) $(HEADERS)
	$(CC) -std=gnu17 -Og -ggdb3 -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment -fanalyzer -v $(SRCS) $(DBLD_FLAGS) -o "$@"

strip: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o  main && strip --strip-all --verbose main

clean:
	rm -f main main-debug
