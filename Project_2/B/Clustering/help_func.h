#ifndef __HELP_FUNC_H__
#define __HELP_FUNC_H__

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
#include <unistd.h>
#include <cstring>

#include "Point.h"


using namespace std;

void To2d(vector<Point>& my_points);

void Snapping(vector<Point>& my_points);

void Padding(vector<Point>& my_points);


double DiscreteFrechet(vector<vector<double>> a, vector<vector<double>> b);

// Return non negative modulo between a and b
long long int modulo(long long int a, long long int b);

// Calculates Euclidean distance between to vectors
long double euc_dist(vector<double > in_vec, vector<double> q_vec);

struct N_near{   
    Point p;
    long double dis;
};

// checks if a is less than b
bool Compare(N_near a, N_near b); 

// $./lsh –i <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
void input_handling(int argc, char *argv[], string& input_file, string& query_file, string& output_file, int& m, int& C, int& update);

int open_files(ifstream& in_file, ifstream& q_file, ofstream& out_file, string& input_file, string& query_file, string& output_file);

void close_files(ifstream& in_file, ifstream& q_file, ofstream& out_file);

int get_input_size(ifstream& input_file);

int get_dim(ifstream& input_file);

void read_conf(ifstream& c_file, string conf_file, int& K, int& L, int& k, int& M_hype, int& k_hype, int& probes);

void Snapping(vector<Point>& my_points);

void Padding(vector<Point>& my_points);

void To2d(vector<Point>& my_points);

vector<vector<double>>  mean_curve(vector<vector<double>> a, vector<vector<double>> b);

vector<vector<double>>  mean_curve_many(vector<vector<vector<double>>> all_curve);



#endif