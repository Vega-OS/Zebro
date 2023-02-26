CFLAGS = -Werror-implicit-function-declaration -pedantic -Iinclude/ -fsanitize=address

.PHONY: all
all: bin/ bin/zebro

bin/:
	mkdir -p bin/

bin/zebro: $(shell find src/ -name "*.c")
	gcc $(CFLAGS) $^ -o $@
