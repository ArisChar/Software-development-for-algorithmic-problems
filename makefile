CC = g++
files = cluster.cpp hyber_cube.cpp lsh.cpp h_hash.cpp Point.cpp help_func.cpp random.cpp
args = -o

cluster_main:
	$(CC) -O2 cluster_main.cpp $(files) $(args) cluster_main

clean:
	rm -rf cluster_main