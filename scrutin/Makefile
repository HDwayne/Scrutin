CC = gcc
CFLAGS = -Wall -std=c99

SRC = src
OBJ = obj
BINDIR = bin

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BIN = $(BINDIR)/scrutin

ZIPNAME=PJS3_SCRUTIN_HERZBERG_DWAYNE_22000086.zip
DOXYCONFIG = Doxyfile

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f -r $(BINDIR)/* $(OBJ)/*

zip:
	zip -r $(ZIPNAME) *

doxygen:
	doxygen $(SRC)/$(DOXYCONFIG)