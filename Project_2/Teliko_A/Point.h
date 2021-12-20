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
 
/*-------------------------Point_Class--------------------------*/

class Point {
    private:
        int pointId;                // Id of point from file
        int clusterId;              
        int dimensions;             // Dimension of point
        int p_metric;

        vector<long long int> IDs;  // Vector that stores the ID(p) from  page 22 of slides
        vector<double> values;      // Vector with that holds the coordinates of a point

        vector<double> curve_3;       // Vector to store pair (x,y) for Aii
        vector<double> filtered;    // Vector to store the filtered timeseries for Aiii

        vector<double> lineToVec2(string &line);

        vector<vector<double>> curve;
        vector<double> curveToVec(string &line);
        
        
    public:
        static int Geniko_id;

        Point(string line, int L, int metric);              // Constructor of point
        Point(vector<double> cen,int cluid);
        void set_ID(long long int id, int pos); // Set ID(p) of point and save it to the vector 
        long long int get_ID(int pos);          // Returns the specified ID(p)
        int getDimensions();                    // Returns the dimension
        int getCluster();
        int getID();                            // Returns id of point
        void setCluster(int val);
        double getVal(int pos);                 // Returns specified coordinate
        vector<double> getCoord();              // Returns the coordinates of the point
        void print_point();                     // prints point
        int getCluster_ID();

        void CreateTuple();
        vector<vector<double>> getCurve();

        void set_value(double value, int index);
        void EraseVal(int index);
        void replace(vector<double>& c);

        void setCurve3(vector<double> a);
        vector<double> getCurve3();
        
        void setFiltered(vector<double>& data);
        vector<double> getFiltered();

        
};

#endif
