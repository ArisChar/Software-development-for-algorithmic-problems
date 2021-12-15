#ifndef __RANDOM_H__
#define __RANDOM_H__

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
#include <omp.h>
#include <cmath>
#include <map>

using namespace std;

// Returns a random number according to a normal distribution with mean 0 and variance 1
double normal_distr(void); 

// Returns a random number according to a uniform distribution with mean 0 and variance W
double uniform_distr(int w); 

// Returns a random integer number according to a uniform distribution with mean 0 and variance INT32_MAX
int random_r();    

#endif