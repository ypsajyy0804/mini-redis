CC := gcc
SANITIZE := 
CFLAGS := -Wall -Wextra -Werror -Isrc -MMD -MP -g
LDFLAGS :=

APP_SRC := src/main.c
LIB_SRCS := src/dict.c src/cmd.c

APP_OBJ := $(patsubst src/%.c,build/%.o,$(APP_SRC))
LIB_OBJS := $(patsubst src/%.c,build/%.o,$(LIB_SRCS))

TEST_SRC := tests/test_dict.c
TEST_OBJ := $(patsubst tests/%.c,build/tests/%.o,$(TEST_SRC))
TEST_BIN := build/tests/test_dict

DEPS = $(APP_OBJ:.o=.d) $(LIB_OBJS:.o=.d) $(TEST_OBJ:.o=.d)
TARGET = build/redis-lite

.PHONY: all test asan clean

all: $(TARGET)

$(TARGET): $(APP_OBJ) $(LIB_OBJS) | build
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $^ $(LDFLAGS) $(SANITIZE)

$(TEST_BIN): $(TEST_OBJ) $(LIB_OBJS) | build/tests
	$(CC) $(CFLAGS) $(SANITIZE) -o $@ $^ $(LDFLAGS) $(SANITIZE)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(SANITIZE) -c $< -o $@

build/tests/%.o: tests/%.c | build/tests
	$(CC) $(CFLAGS) $(SANITIZE) -c $< -o $@

build build/tests:
	mkdir -p $@

test: $(TEST_BIN)
	./$(TEST_BIN)

asan:
	$(MAKE) clean
	$(MAKE) SANITIZE="-fsanitize=address,undefined" all $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf build

-include $(DEPS)
