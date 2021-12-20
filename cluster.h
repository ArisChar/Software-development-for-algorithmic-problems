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
    vector<vector<double>> curve_centroid;
    vector<Point> points;

public:
    Cluster(int clusterId, Point centroid);
    Cluster(int clusterId, Point centroid ,int metrikh);
    void addPoint(Point p);
    bool removePoint(int pointId);
    void removeAllPoints();
    int get_clusterId();
    Point getPoint(int pos);
    int getSize();
    double getCentroidByPos(int pos);
    double getCurvedByPos(int pos);
    vector<double> getCentroid();
    void setCentroidByPos(int pos, double val);
    vector<vector<double>> getCurve_Centroid();
    vector<vector<double>> getCurve_Curve_index(int index);
    void setCurve_Centroid(vector<vector<double>> &a);
    double getCurvedByPos_x(int pos);
    double getCurvedByPos_y(int pos);
    int getSize_Curve();
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

    int getNearestClusterId_DFD(Point point);

    long double NearestClusterDistance(Point point);

    long double NearestClusterDistance_DFD(Point point);

public:
    KMeans(int K, int iterations, string output_dir);
    void run_Lloyd(vector<Point> &all_points,int c);
    void run_LSH(vector<Point> &all_points,int k_hash,int L_hash,int c);
    void run_CUBE(vector<Point> &all_points,int d,int m_buckets ,int probes, int c);
    void run_Lloyd_DFD(vector<Point> &all_points, int c);
    void run_LSH_DFD(vector<Point> &all_points,int k_hash,int L_hash, int c);
};

static bool sort_using(double u, double v);