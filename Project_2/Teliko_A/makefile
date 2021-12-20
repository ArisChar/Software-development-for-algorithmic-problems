CC = g++
files = lsh.cpp h_hash.cpp Point.cpp help_func.cpp random.cpp hyber_cube.cpp
fred = ./Fred/config.cpp ./Fred/curve.cpp ./Fred/frechet.cpp ./Fred/interval.cpp ./Fred/point.cpp ./Fred/simplification.cpp
# CFLAGS = -O2 
args = -o

main:
	$(CC) -O2 main.cpp  $(files) $(fred) $(args) main

test:
	$(CC) test.cpp  $(files) $(fred) $(args) test

clean:
	rm -rf main test