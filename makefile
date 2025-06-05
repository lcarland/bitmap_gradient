HEADERS = main.h
SRC_DIR := src
OUT_DIR := bin

default: app

app:
	mkdir -p $(OUT_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OUT_DIR)/bmpgen

test:
	mkdir -p $(OUT_DIR)
	gcc -c $(SRC_DIR)/test.c -o $(OUT_DIR)/test

clean:
	-rm -f bin/*
