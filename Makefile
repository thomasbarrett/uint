CC=clang
CFLAGS = -std=c11 -Iinclude
SRC_FILES = example
OBJ_FILES = $(addprefix obj/,$(SRC_FILES:=.o))
TESTS = $(addprefix bin/test_,$(SRC_FILES))

.PHONY: all
all: $(TESTS)

.PHONY: clean
clean:
	rm -rf obj
	rm -rf bin

obj:
	mkdir obj

bin:
	mkdir bin

.PHONY: build
build: $(TESTS)
	rm -rf obj

obj/%.o: src/%.c | obj
	$(CC) -c $(CFLAGS) $^ -o $@

bin/test_%: tests/test_%.c $(OBJ_FILES) | bin
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: test
test: $(TESTS)
	@for t in $(TESTS); do echo $$t; $$t || exit 1; done
