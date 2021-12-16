#ifndef __H_HASH_H__
#define __H_HASH_H__

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
#include "help_func.h"
#include "random.h"

using namespace std;

class h_hash{
    private:
        double t;          // Holds the random real t needed for the h(p)
        vector<double> v;  // Holds the random vector for the h(p)
        int w, dim;        // Holds the w "window" and dimension
    
    public:
        h_hash(int dim);        // Constructor of h_hash       
        long long int hash_function(Point& p); // Computes the h(p) given a point
        double inner_product(vector<double> a, vector<double> b); // Computes the inner product of two vectors
        vector<double> get_v(); // Returns the vector v
        int get_t();            // Returns t 
        int get_w();            // Returns w 
};

#endif