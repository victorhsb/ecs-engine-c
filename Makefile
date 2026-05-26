CC := cc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g
CPPFLAGS := -Iexternal/raylib/src -Isrc
LDFLAGS := -Lexternal/raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC := $(wildcard src/*.c)
OBJ := $(patsubst %.c, %.o, $(SRC))

app: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) app

.PHONY: clean
