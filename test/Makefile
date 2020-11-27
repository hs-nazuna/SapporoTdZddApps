# Path to TdZdd
TDZDD   = ../../libs/TdZdd-master
# Path to SAPPOROBDD
SAPPORO = ../../libs/SAPPOROBDD20180619
# Path to SapporoTdZddApps
APPS = ../

CC      = g++

LIB     = $(SAPPORO)/lib/BDD.a
LIB64   = $(SAPPORO)/lib/BDD64.a
INCLUDE = -I$(TDZDD)/include -I$(SAPPORO)/include -I$(APPS)/include

PRG     = test
PRG64   = test64

OPT     = -std=c++17 -O3 $(INCLUDE) -Wall
OPT64   = $(OPT) -DB_64
OBJ     = test.o
OBJ64   = test64.o
HPP     = *.hpp

all: $(PRG)

64: $(PRG64)

$(PRG): $(OBJ) $(LIB)
	$(CC) $(OPT) $(OBJ) $(LIB) -o $(PRG)

$(PRG64): $(OBJ64) $(LIB64)
	$(CC) $(OPT64) $(OBJ64) $(LIB64) -o $(PRG64)

$(OBJ): $(PRG).cpp $(HPP)
	$(CC) $(INCLUDE) $(OPT) -c $(PRG).cpp -o $(OBJ)

$(OBJ64): $(PRG).cpp $(HPP)
	$(CC) $(INCLUDE) $(OPT64) -c $(PRG).cpp -o $(OBJ64)

clean:
	rm -f $(PRG) $(OBJ) $(PRG64) $(OBJ64)
