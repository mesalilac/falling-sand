CC=gcc
CFLAGS=-Wall -Wextra -Wformat -pedantic -ggdb
IDIR=include
INCLUDE=-I$(IDIR)/
LIBS= -lSDL2 -lSDL2_ttf
SRCS=main.c
OUT=falling-sand

build:
	$(CC) $(CFLAGS) $(INCLUDE) -o $(OUT) $(SRCS) $(LIBS)

run: build
	./$(OUT)
