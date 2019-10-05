CC = gcc
CFLAGS = -Wall -g -std=c11

BIN = bin/
SRC = src/

all: $(BIN)LinkedListAPI.o $(BIN)GEDCOMparser.o $(BIN)GEDCOMutilities.o
	$(CC) -shared -o $(BIN)parser.so $(BIN)LinkedListAPI.o $(BIN)GEDCOMparser.o $(BIN)GEDCOMutilities.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c
	$(CC) $(CFLAGS) -fpic -Iinclude -c $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

$(BIN)GEDCOMparser.o: $(SRC)GEDCOMparser.c
	$(CC) $(CFLAGS) -fpic -Iinclude -c $(SRC)GEDCOMparser.c -o $(BIN)GEDCOMparser.o

$(BIN)GEDCOMutilities.o: $(SRC)GEDCOMutilities.c
	$(CC) $(CFLAGS) -fpic -Iinclude -c $(SRC)GEDCOMutilities.c -o $(BIN)GEDCOMutilities.o

clean:
	rm -f $(BIN)*o $(BIN)*a