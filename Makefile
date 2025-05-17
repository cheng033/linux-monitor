CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude
LDFLAGS=-lncurses

SRC=src/main.c src/cpu.c src/memory.c src/disk.c
TARGET=monitor

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o

