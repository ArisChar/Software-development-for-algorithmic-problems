#include "lsh.h"
#include "help_func.h"
#include "hyber_cube.h"

using namespace std;

int main(int argc, char **argv){

    srand(time(0));

    int k = 14;
    int m = 10;
    int N = 1;
    int R = 10000;
    int p = 2;

    // Input File 
    ifstream in_file; 
    string input_file;

    // Query file
    ifstream q_file; 
    string query_file;

    // Output gile
    ofstream out_file; 
    string output_file;
    
    input_handling(argc, argv, input_file, query_file, output_file, k, m, N, R, p);

    int file = open_files(in_file, q_file, out_file, input_file, query_file, output_file);
    if(file == -1){
        return -1;
    }

    int size = get_input_size(in_file);
    cout<<" Size of data is: "<<size<<endl;

    int dim = get_dim(in_file);
    cout<<" Dimension of data is: "<<dim<<endl;

    // Get all input points from file
    vector<Point> all_points;
    string line;
    int L = 1;
    while (getline(in_file, line)) {
        Point point(line, L);
        all_points.push_back(point);
    }
    Point::Geniko_id=0;
    // Get all query points from file
    vector<Point> q_points;
    string q_line;
    while (getline(q_file, q_line)){
        Point point(q_line, L);
        q_points.push_back(point);
    }

    cout<<" Peforming lsh with :\n"
        <<" | "<<"k = "<<k
        <<" | "<<"M = "<<m
        <<" | "<<"N = "<<N
        <<" | "<<"probes = "<<p
        <<" | "<<"R = "<<R<<" |"<<endl;

    hypercube hype(k,dim);
    hype.hypercube_input(all_points);
    cout <<"ok 1\n";
    hype.hyper_search_N(q_points, N, m, p, out_file);
    cout <<"ok 2\n";
    ofstream range_out; 
    string range_s = "cube_R_search";
    range_out.open(range_s);
    hype.hyper_search_R_All(q_points, R, m, p, range_out);
    cout <<"ok 3\n";
    close_files(in_file, q_file, out_file);
    range_out.close();
    
    return 0;
}