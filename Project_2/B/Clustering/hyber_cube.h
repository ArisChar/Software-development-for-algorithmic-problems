#ifndef __HYPER_CUBE_H__
#define __HYPER_CUBE_H__

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <float.h>
#include <sstream>
#include <vector>
#include <random>
#include <time.h>
#include <chrono>
#include <string>
#include <omp.h>
#include <cmath>
#include <map>
#include "lsh.h"

using namespace std;

class f_hash{
    private:
        map<long long int, int> f_pair;
        vector<h_hash> h;
    public:
        f_hash(int dim);
        int f_function(Point p);
};

class hypercube{
    private:
        int k;
        int d;
        vector<f_hash>  f_number;
        vector<vector<Point>> hypercube_table;
    public:
        hypercube(int k, int dim);
        void hypercube_input(vector<Point> all_points);
        void print_hyper();
        void hyper_search_N(vector<Point> query_points, int N, int m, int probes, ofstream& out_file);
        void hyper_search(Point query, int N, int m, int probes, ofstream& out_file);
        void hyper_search_R(Point query, int R, int m, int probes, ofstream& out_file);
        void hyper_search_R_All(vector<Point> query_points, int R, int m, int probes, ofstream& out_file);
        Apotelesma_range hyper_search_R_clu(Point query, int R, int m, int probes);
};

void hamming_dis_near(vector<char>& str, const int i, const int changesLeft,vector<string>& str_re);

#endif