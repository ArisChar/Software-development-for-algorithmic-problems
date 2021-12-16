#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

//function to display the list 
long double euc_dist(vector<double > in_vec, vector<double> q_vec){
    long double  dist = 0;

    for(int i=0; i<in_vec.size(); i++){
        dist = dist + pow(in_vec[i]-q_vec[i],2);
    }

    return dist;
}

int main()
{
    vector<int> c = {1,1,1,1,1,1,1,1,1,1,2};
    vector<int> target;
    int minima;
    int maxima;
    int sz = c.size();
    for(int i =0; i< c.size(); i++){

        minima = min(c[i],c[i+2]);
        maxima = max(c[i],c[i+2]);

        cout<<c[i]<<"|"<<c[i+1]<<"|"<<c[i+2];

        if( minima <= c[i+1] && c[i+1] <= maxima){
            cout<<"->"<<c[i+1];
            target.push_back(c[i]);
            i++;
            //  c.erase(c.begin()+i+1);
        }else{
            target.push_back(c[i]);
        }
        cout<<endl;

    }

    for(int i =0; i<target.size(); i++){
        cout<<target[i];
    } 
	
	return 0;
}

	// vector<vector<double>> a = {{0.0, 0.0},{1.0,0.0},{2.0,0.0},{3.0,0.0},{4.0,0.0}};
    // vector<vector<double>> b = {{0.0, 1.0},{1.0,1.1},{2.0,1.2},{3.0,1.1},{4.0,1.0}};
    // vector<int> c = {1,3,2,3,4,5,6,8};
    // int e = 2;

    // for(int i =0; i<c.size()-3; i++){
    //     if(abs(c[i]-c[i+1]) <= e && abs(c[i+1]-c[i+2]) <= e){
    //         c.erase(c.begin()+i+1);
    //     }
    // }

    // for(int i =0; i<c.size(); i++){
    //     cout<<c[i];
    // } 
	

	// vector<vector<double>> a = {{0.0, 0.0},{1.0,0.0},{2.0,0.0},{3.0,0.0},{4.0,0.0}};
    // vector<vector<double>> b = {{0.0, 1.0},{1.0,1.1},{2.0,1.2},{3.0,1.1},{4.0,1.0}};

    // vector<vector<double>> d;
    // d.resize(5, vector<double>((5)));

    // for (int i = 0; i < a.size(); ++i) {

    //     for (int j = 0; j < b.size(); ++j) {

    //         d[i][j] = euc_dist(a[i],b[j]);
            
    //     }
    // }

    // vector<vector<double>> c;
    // c.resize(5, vector<double>((5)));

    // for (int i = 0; i < a.size(); ++i) {

    //     for (int j = 0; j < b.size(); ++j) {

    //         if(i == 0 && j == 0){
    //             c[i][j] = d[i][j];
    //         }else if (i == 0 && j > 0){
    //             c[i][j] = max(c[i][j-1],d[i][j]);
    //         }else if (i > 0 and j == 0){
    //             c[i][j] = max(c[i-1][j],d[i][j]);
    //         }else{
    //             c[i][j] = max(min(min(c[i-1][j], c[i-1][j-1]), c[i][j-1]), d[i][j]);
    //         }
    //     }
        
    // }

    // cout<<sqrt(c[4][4]);