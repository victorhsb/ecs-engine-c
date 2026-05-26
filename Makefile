CC := cc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g
CPPFLAGS := -I/opt/homebrew/Cellar/raylib/5.5/include -Isrc
LDFLAGS := -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib

SRC := src/*.c
OBJ := $(SRC:.c=.o)

app: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) app
