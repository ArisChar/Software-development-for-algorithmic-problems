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
#include "Fred/frechet.hpp"

using namespace std;

long double Distance(Point in, Point q, int metric);

void Snapping(vector<Point>& my_points, double delta);

void Padding(vector<Point>& my_points);

vector<Point> Filter(vector<Point>& my_points);

void To2d(vector<Point>& my_points, double delta);

void SnappingDiscrete(vector<Point>& my_points, double d);

void PaddingDiscrete(vector<Point>& my_points);

void To2dDiscrete(vector<Point>& my_points, double delta);

void PutFiltered(vector<Point>& target_points, vector<Point>& filtered_points);

long double DiscreteFrechet(vector<vector<double>> a, vector<vector<double>> b);

long double ContinuousFrechet(vector<double> a, vector<double> b);

// Calculates Euclidean distance between to vectors
long double euc_dist(vector<double> in_vec, vector<double> q_vec);

// Return non negative modulo between a and b
long long int modulo(long long int a, long long int b);

struct N_near{   
    Point p;
    long double dis;
};

void PutFiltered(vector<Point>& target_points, vector<Point>& filtered_points);

// checks if a is less than b
bool Compare(N_near a, N_near b); 

// $./lsh –i <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
void input_handling(int argc, char *argv[], string& input_file, string& query_file, string& output_file, int& k, int& L, int& N, int& m, int& probes, int& algor, int& metric, double& d, int& K);

int open_files(ifstream& in_file, ifstream& q_file, ofstream& out_file, string& input_file, string& query_file, string& output_file);

void close_files(ifstream& in_file, ifstream& q_file, ofstream& out_file);

int get_input_size(ifstream& input_file);

int get_dim(ifstream& input_file);

#endif