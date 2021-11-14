#ifndef __LSH_H__
#define __LSH_H__

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <float.h>
#include <vector>
#include <random>
#include <time.h>
#include <chrono>
#include <string>
#include <sstream>
#include <omp.h>
#include <cmath>
#include <map>
#include "Point.h"
#include "h_hash.h"
#include "help_func.h"
#include "random.h"

#define M 4294967291

using namespace std;

/*-------------------------g_hash--------------------------*/

class g_hash {
    private:
        int k;                      // Holds the number of h we'll make
        vector<h_hash> hashTable;   // Vector that holds the hi(p)
        vector<int> r;              // The random r needed for each h
        
    public:
        g_hash(int k, int dim);     // Inizializes the k h function and k random r
        long long int g_function(Point& p, int n, int pos); // Given a point calculates g(p)
};

/*-------------------------lsh_hash--------------------------*/

struct Apotelesma_range
{   
    vector<int> kanonika_apot;
    vector<int> bloblhmatika_apot;
    vector<int> bloblh_kentro;

};

class lsh{
    private:
        int k, L, dim, size;                         // size - total entries
        vector<g_hash> g_tables;                     // Vector that holds L g(p) 
        vector<vector<vector<Point>>> G_Hash_Tables; // 3d vector to store all the points
    public:
        lsh(int k, int L, int dim, int size);                                                       // Initialize the hashtable
        void lsh_tables_create(vector<Point>& all_points);                                          // Inputs all the points to the L different g 
        long long int hash_query(Point& query_point, int g_func);                                   // Given a query point finds its hash value
        void query_search(vector<Point> query_points, vector<Point> input_points, ofstream& out_file);            // Given a query point finds its nearest neighbour
        double brute_force(Point query_point, vector<Point> input_points, ofstream& out_file);                    // Given a query point finds its nearest neighbour by brute force
        void query_range_search(vector<Point> query_points, vector<Point> input_points, double R, ofstream& out_file);                // Range search of nearest neighbour
        void query_search_N(vector<Point> query_points, vector<Point> input_points, int N, ofstream& out_file);   // Given a query point finds N nearest neighbours
        void print(void); 
        Apotelesma_range query_range_search_clu(Point query_points,double R);
}; 

#endif