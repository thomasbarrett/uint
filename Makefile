CC=clang
CFLAGS = -std=c11 -Iinclude -fprofile-instr-generate -fcoverage-mapping
SRC_FILES = add sub
OBJ_FILES = $(addprefix obj/,$(SRC_FILES:=.o))
TESTS = $(addprefix bin/test_,$(SRC_FILES))

export PATH := $(PATH):$(LLVM_PATH)

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
build: $(TESTS) $(OBJ_FILES)

obj/%.o: src/%.c | obj
	$(CC) -c $(CFLAGS) $^ -o $@

bin/test_%: tests/test_%.c $(OBJ_FILES) | bin
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: test
test: $(TESTS)
	echo $(PATH)
	@for t in $(TESTS); do echo $$t; LLVM_PROFILE_FILE=profile/%p.profraw $$t || exit 1; done
	@llvm-profdata merge -sparse profile/*.profraw -o coverage.profdata
	@llvm-cov report --instr-profile=coverage.profdata -object obj/*.o
	@rm -rf profile
	@rm coverage.profdata
