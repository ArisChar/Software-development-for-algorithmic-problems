#include <omp.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <chrono>
#include <float.h>
#include <string>

#include "Point.h"
#include "h_hash.h"
#include "help_func.h"
#include "random.h"
#include "lsh.h"
#include "hyber_cube.h"

using namespace std;

class Cluster
{
private:
    int clusterId;
    vector<double> Cluster_centroid;
    vector<Point> points;

public:
    Cluster(int clusterId, Point centroid);
    void addPoint(Point p);
    bool removePoint(int pointId);
    void removeAllPoints();
    int get_clusterId();
    Point getPoint(int pos);
    int getSize();
    double getCentroidByPos(int pos);
    vector<double> getCentroid();
    void setCentroidByPos(int pos, double val);
};
//-------------------KMeans------------------
class KMeans
{
private:
    int K, iters;
    vector<Cluster> clusters;
    string output_dir;

    void clearClusters();
    int getNearestClusterId(Point point);
    long double NearestClusterDistance(Point point);

public:
    KMeans(int K, int iterations, string output_dir);
    void run_Lloyd(vector<Point> &all_points,int c);
    void run_LSH(vector<Point> &all_points,int k_hash,int L_hash,int c);
    void run_CUBE(vector<Point> &all_points,int d,int m_buckets ,int probes, int c);
};

static bool sort_using(double u, double v);