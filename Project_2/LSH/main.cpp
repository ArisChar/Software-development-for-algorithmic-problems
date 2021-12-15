#include "lsh.h"
#include "help_func.h"

using namespace std;

int main(int argc, char **argv){

    srand(time(0));

    int k = 4;
    int L = 5;
    int N = 1;
    int R = 10000;

    // Input File 
    ifstream in_file; 
    string input_file;

    // Query file
    ifstream q_file; 
    string query_file;

    // Output gile
    ofstream out_file; 
    string output_file;
    
    input_handling(argc, argv, input_file, query_file, output_file, k, L, N, R);

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
        <<" | "<<"L = "<<L
        <<" | "<<"N = "<<N
        <<" | "<<"R = "<<R<<" |"<<endl;


    lsh l(k,L,dim,size);
    l.lsh_tables_create(all_points);

    l.query_search(q_points, all_points, out_file);

    ofstream range_out; 
    string range_s = "range_search";
    range_out.open(range_s);
    l.query_range_search(q_points, all_points, R, range_out);

    if(N > 1){
        ofstream N_near_out; 
        string N_near_file = "N_nearest";
        N_near_out.open(N_near_file);
        l.query_search_N(q_points, all_points, N, N_near_out);

        N_near_out.close();
    }
    
    close_files(in_file, q_file, out_file);
    range_out.close();
    
    return 0;
}