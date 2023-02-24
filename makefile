# Author: Vishwa Venkateshwaran, Nathalie Kaspar

all: DSSimul

DSSimul: DSSimul.c DSSimul.h
	gcc DSSimul.c -o DSSimul

cleanO:
	rm -rf *.o

clean:
	rm -rf DSSimul *.o
