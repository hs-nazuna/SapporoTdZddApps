CC		= g++

TDZDD	= -I../../libs/TdZdd-master/include	# Path to TdZdd
SAPPORO = -I../../libs/SAPPOROBDD20180619/include # Path to SAPPOROBDD
PRG     = test
OPT 	= -std=c++17 -O3 $(TDZDD) $(SAPPORO) -Wall
OBJ		= test.o
HPP		= *.hpp

all:	$(PRG)

$(PRG): $(OBJ)
	$(CC) $(OPT) $(OBJ) -o $(PRG)

test.o: test.cpp $(HPP)
	$(CC) $(INCLUDE) $(OPT) -c test.cpp

clean:
	rm -f test $(OBJ)
