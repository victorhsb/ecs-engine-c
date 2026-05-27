CC := cc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g
CPPFLAGS := -Iexternal/raylib/src -Isrc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
LDFLAGS := -Lexternal/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif
ifeq ($(UNAME_S),Darwin)
LDFLAGS := -Lexternal/raylib/src -lraylib -lm -lpthread -ldl -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif

SRC := $(wildcard src/*.c)
OBJ := $(patsubst %.c, %.o, $(SRC))

TEST_DEPS := src/entity.o src/storage.o
TEST_RUNNER_SRC := $(wildcard test/*.c)
TEST_RUNNER_OBJ := $(patsubst %.c, %.o, $(TEST_RUNNER_SRC))
UNITY_OBJ := external/unity/unity.o

TEST_CPPFLAGS := -Iexternal/unity -Isrc -Iexternal/raylib/src
TEST_BIN := bin/test_runner

app: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

test: $(TEST_DEPS) $(UNITY_OBJ) $(TEST_RUNNER_OBJ)
	@mkdir -p bin
	$(CC) $(TEST_DEPS) $(UNITY_OBJ) $(TEST_RUNNER_OBJ) -lm -o $(TEST_BIN)
	./$(TEST_BIN)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

external/unity/unity.o: external/unity/unity.c
	$(CC) $(TEST_CPPFLAGS) $(CFLAGS) -c $< -o $@

test/%.o: test/%.c
	$(CC) $(TEST_CPPFLAGS) $(CFLAGS) -c $< -o $@

raylib:
	git submodule update --init --recursive
	$(MAKE) -C external/raylib/src

clean:
	rm -f $(OBJ) app

cleantest:
	rm -f $(TEST_DEPS) $(UNITY_OBJ) $(TEST_RUNNER_OBJ) $(TEST_BIN)

.PHONY: clean cleantest raylib test
