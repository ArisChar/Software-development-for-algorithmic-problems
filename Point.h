#ifndef __POINT_H__
#define __POINT_H__

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



using namespace std;

vector <double> tuble_to_vector(vector<vector<double>> a);

/*-------------------------Point_Class--------------------------*/


class Point {
    private:
        int pointId;                // Id of point from file
        int clusterId;
        int dimensions;             // Dimension of point
        vector<long long int> IDs;  // Vector that stores the ID(p) from  page 22 of slides
        vector<double> values;      // Vector with that holds the coordinates of a point
        vector<vector<double>> curve;

        vector<double> lineToVec2(string &line ,int &id);
        vector<double> curveToVec(string &line);

    public:
        Point(string line, int L);              // Constructor of point
        Point(vector<double> cen,int cluid);
        Point(string line, int L,int metrikh);
        Point(vector<vector<double>> cen,int cluid,int metrikh);
        void set_ID(long long int id, int pos); // Set ID(p) of point and save it to the vector 
        long long int get_ID(int pos);          // Returns the specified ID(p)
        int getDimensions();                    // Returns the dimension
        int getID();                            // Returns id of point
        void setCluster(int val);
        double getVal(int pos);                 // Returns specified coordinate
        vector<double> getCoord();              // Returns the coordinates of the point
        void setVal(int pos ,int val);
        void print_point();
        int getCluster_ID();

        void set_value(double value, int index);
        void CreateTuple();
        vector<vector<double>> getCurve();
        static int Geniko_id;
};



#endif