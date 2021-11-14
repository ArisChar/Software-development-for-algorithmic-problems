#include "lsh.h"
#include "help_func.h"
#include "hyber_cube.h"
#include "cluster.h"

using namespace std;

int main(int argc, char **argv){

    srand(time(0));
    int C = 0;
    int m = 0;
    int K = 5;
    int L = 3;
    int k = 4;
    int M_hype = 10;
    int k_hype = 3;
    int probes = 2;

    // Input File 
    ifstream in_file; 
    string input_file;

    // Query file
    ifstream c_file; 
    string conf_file;

    // Output gile
    ofstream out_file; 
    string output_file;
    
    input_handling(argc, argv, input_file, conf_file, output_file, m, C);

    int file = open_files(in_file, c_file, out_file, input_file, conf_file, output_file);
    if(file == -1){
        return -1;
    }

    read_conf(c_file, conf_file, K, L, k, M_hype, k_hype, probes);

    int size = get_input_size(in_file);
    cout<<" Size of data is: "<<size<<endl;

    int dim = get_dim(in_file);
    cout<<" Dimension of data is: "<<dim<<endl;

    // Get all input points from file
    vector<Point> all_points;
    string line;

    while (getline(in_file, line)) {
        Point point(line, L);
        all_points.push_back(point);
    }

    int iters = 100;

    KMeans kmeans(K, iters, output_file);
    
    cout<<" Parameters are:\n"
        <<" | "<<"K = "<<K
        <<" | "<<"k = "<<k
        <<" | "<<"L = "<<L
        <<" | "<<"probes = "<<probes
        <<" | "<<"k_hype = "<<k_hype
        <<" | "<<"M_hype = "<<M_hype<<" |"<<endl;

    if(m == 0){
        cout<<" Lloyd"<<endl;
        kmeans.run_Lloyd(all_points, C);
    }else if(m == 1){
        cout<<" LSH"<<endl;
        kmeans.run_LSH(all_points, k, L, C);
    }else if(m == 2){
        cout<<" Cube"<<endl;
        kmeans.run_CUBE(all_points, k_hype, M_hype, probes, C);
    }

    close_files(in_file, c_file, out_file);
    
    return 0;
}

// ./cluster_main -i input_small_id -c cluster.conf -o output_file