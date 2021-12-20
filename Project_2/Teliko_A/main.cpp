#include "lsh.h"
#include "help_func.h"
#include "Fred/frechet.hpp"
#include "hyber_cube.h"

using namespace std;

int main(int argc, char **argv){

    srand(time(0));

    // Default values
    int k = 1;
    int L = 1;
    int N = 10;
    int R = 10000;
    int m = 10;
    int probes = 2;
    int K = 14;
    
    // Metric    
    int algor = 0;      // | default 0 for LSH | 1 for Hypercube | 2 for Frechet
    int metric = 0;     // | default 0 for L2  | 1 for discrete  | 2 for continuous
    double delta = 1;   // default delta

    // Input File 
    ifstream in_file; 
    string input_file;

    // Query file
    ifstream q_file; 
    string query_file;

    // Output gile
    ofstream out_file; 
    string output_file;
    
    input_handling(argc, argv, input_file, query_file, output_file, k, L, N, m, probes, algor, metric, delta, K);

    int file = open_files(in_file, q_file, out_file, input_file, query_file, output_file);
    if(file == -1){
        return -1;
    }

    int size = get_input_size(in_file);
    cout<<"\n Size of data is: "<<size<<endl;

    int dim = get_dim(in_file);
    cout<<"\n Dimension of data is: "<<dim<<endl;

    // Get all input points from file
    vector<Point> all_points;
    string line;
    while (getline(in_file, line)) {
        Point point(line, L, metric);
        all_points.push_back(point);
    }

    // Get all query points from file
    Point::Geniko_id=0;
    vector<Point> q_points;
    string q_line;
    while (getline(q_file, q_line)){
        Point point(q_line, L, metric);
        q_points.push_back(point);
    }

    out_file<<"Algorithm: ";
    cout<<"\n Algorithm: ";
    
    if(algor == 0)      { 
        out_file<<"LSH_Vector\n";
        cout<<"LSH\n"
            <<" | "<<"k = "<<k
            <<" | "<<"L = "<<L<<" |\n"; 
    }
    else if(algor == 1) {
        out_file<<"Hypercube\n";
        cout<<"Hypercube\n"
            <<" | "<<"k(=d') = "<<K
            <<" | "<<"M = "<<m
            <<" | "<<"Probes = "<<probes<<" |\n"; 
    }
    else if(algor == 2) { 
        cout<<"Frechet\n"
            <<" | "<<"k = "<<k
            <<" | "<<"L = "<<L
            <<" | "<<"delta = "<<delta<<" |\n"; 
        cout<<" Metric: ";
        if(metric == 1){ 
            out_file<<"LSH_Frechet_Discrete\n";
            cout<<"Discrete\n";
            To2dDiscrete(all_points, delta);
            To2dDiscrete(q_points, delta);
            dim *= 2;
        }
        else if(metric == 2){
            out_file<<"LSH_Frechet_Continuous\n";
            cout<<"Continuous\n";
            To2d(all_points, delta);
            To2d(q_points, delta);
        }
        else{ 
            cout<<"No metric for Frechet\n"; 
            exit(EXIT_FAILURE);
        }
    }

    //-----------------------------------------------------

    if(algor == 1){
        hypercube hype(K,dim);
        hype.hypercube_input(all_points);
        hype.hyper_search_N(q_points, N, m, probes, out_file);
    }else{
        lsh l(k,L,dim,size,metric);
        l.lsh_tables_create(all_points);
        l.query_search(q_points, all_points, out_file);        
    }

    // if(N > 1){
    //     ofstream N_near_out; 
    //     string N_near_file = "N_nearest";
    //     N_near_out.open(N_near_file);

    //     l.query_search_N(q_points, all_points, N, N_near_out);

    //     N_near_out.close();
    // }

    // //---------------------Range-Search-----------------------

    // ofstream range_out; 
    // string range_s = "range_search";
    // range_out.open(range_s);

    // l.query_range_search(q_points, all_points, R, range_out);

    // range_out.close();

    //---------------------Close files-----------------------

    close_files(in_file, q_file, out_file);
    
    return 0;
}