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

app: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

raylib:
	git submodule update --init --recursive
	$(MAKE) -C external/raylib/src

clean:
	rm -f $(OBJ) app

.PHONY: clean raylib
