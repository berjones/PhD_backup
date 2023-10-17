CC = g++
CFLAGS = -O2 -pedantic -std=c++14 -Wall -Iinclude -fopenmp

compare_values.exe: compare_values.cc
	$(CC) $(CFLAGS) -o compare_values.exe compare_values.cc

.PHONY: clean

clean:
	rm -f compare_values.exe
