CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Icore/include

TARGET = shell-done
BUILD_DIR = build

SRCS = core/src/main.c core/src/parser.c core/src/executor.c
OBJS = $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: core/src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean