CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude
LDFLAGS=-lncurses

SRC=src/main.c src/cpu.c src/memory.c src/disk.c src/log.c
TARGET=monitor

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)


# 增加這些
tests/test_cpu: tests/test_cpu.c src/cpu.c
	$(CC) -Iinclude $^ -o $@
tests/test_mem: tests/test_mem.c src/memory.c
	$(CC) -Iinclude $^ -o $@
.PHONY: test
test: tests/test_cpu tests/test_mem
	@echo "Running all tests..."
	@tests/test_runner.sh



clean:
	rm -f $(TARGET) src/*.o

