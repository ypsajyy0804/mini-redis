CC = gcc
CFLAGS = -Wall -Wextra -Werror -Isrc -MMD -MP
SRCS = src/main.c src/dict.c src/cmd.c
OBJ = $(patsubst src/%.c,build/%.o,$(SRCS))
DEPS = $(OBJ:.o=.d)
TARGET = build/redis-lite

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | build
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

-include $(DEPS)

clean:
	rm -rf build
