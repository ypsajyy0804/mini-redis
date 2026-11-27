CC := gcc
CFLAGS := -Wall -Wextra -Werror -Isrc -MMD -MP
LDFLAGS :=

APP_SRC := src/main.c
LIB_SRCS := src/dict.c src/cmd.c

APP_OBJ := $(patsubst src/%.c,build/%.o,$(APP_SRC))
LIB_OBJS := $(patsubst src/%.c,build/%.o,$(LIB_SRCS))

TEST_SRC := tests/leak_demo.c
TEST_OBJ := $(patsubst tests/%.c,build/tests/%.o,$(TEST_SRC))
TEST_BIN := build/tests/leak_demo

DEPS = $(APP_OBJ:.o=.d) $(LIB_OBJS:.o=.d) $(TEST_OBJ:.o=.d)
TARGET = build/redis-lite

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(APP_OBJ) $(LIB_OBJS) | build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_BIN): $(TEST_OBJ) $(LIB_OBJS) | build/tests
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/tests/%.o: tests/%.c | build/tests
	$(CC) $(CFLAGS) -c $< -o $@

build build/tests:
	mkdir -p $@

test: $(TEST_BIN)
	./$(TEST_BIN)

-include $(DEPS)

clean:
	rm -rf build
