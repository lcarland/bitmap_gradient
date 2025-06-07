HEADERS = main.h
SRC := src
OUT := bin

$(shell mkdir -p $(OUT))

default: app

app:
	gcc $(SRC)/main.c $(SRC)/configure.c -o $(OUT)/bmpgen

debug:
	gcc -g $(SRC)/main.c $(SRC)/configure.c -o $(OUT)/bmpgen

clean:
	-rm -f bin/*
	-rm -f output.bmp
