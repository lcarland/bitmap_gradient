HEADERS = main.h
SRC_DIR := src
OUT_DIR := bin
CFLAGS := -g

default: app

app:
	mkdir -p $(OUT_DIR)
	gcc $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/configure.c -o $(OUT_DIR)/bmpgen

test:
	mkdir -p $(OUT_DIR)
	gcc $(SRC_DIR)/test.c -o $(OUT_DIR)/test

clean:
	-rm -f bin/*
	-rm -f output.bmp
