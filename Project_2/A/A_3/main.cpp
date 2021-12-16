#include "lsh.h"
#include "help_func.h"

using namespace std;

void Snapping(vector<Point>& my_points){
    int d = 2;
    //double tx = uniform_distr(d);
    double ty = uniform_distr(d);

    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j++){

            double y = my_points[i].getVal(j);
            double new_y = (floor(((y+ty)/d))*d)-d;
            my_points[i].set_value(new_y,j); 

        }
    }
}

void Padding(vector<Point>& my_points){
    vector<double> target;

    double max_y = 10000.0;
    int minima;
    int maxima;
    double x1,x2,x3;

    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j++){ 
            x1 = my_points[i].getVal(j);
            x2 = my_points[i].getVal(j+1);
            x3 = my_points[i].getVal(j+2);   

            minima = min(x1,x3);
            maxima = max(x1,x3);
            
            if( minima <= x2 && x2 <= maxima){
                target.push_back(x1);
                j++;
                if(my_points.size()-2 == i){
                    target.push_back(x1);
                    target.push_back(x2);
                    target.push_back(x3);
                    break;
                }
            }else{
                target.push_back(x1);
                if(my_points.size()-2 == i){
                    target.push_back(x1);
                    target.push_back(x2);
                    target.push_back(x3);
                }
            }

        }

        for(int j=0; j<my_points[i].getDimensions(); j++){  

            if( j < target.size() ){
                my_points[i].set_value(target[j],j);
            }else{
                my_points[i].set_value(max_y,j);
            }

        }

        target.clear();
    }
}

void Filter(vector<Point>& my_points){
    double e = 0.15;
    double x1,x2,x3;

    for(int i =0; i<my_points.size(); i++){
        
        
        for(int j=0; j<my_points[i].getCoord().size()-2; j++){
            x1 = my_points[i].getVal(j);
            x2 = my_points[i].getVal(j+1);
            x3 = my_points[i].getVal(j+2);

            if(abs(x1-x2) <= e && abs(x2-x3) <= e){
                my_points[i].EraseVal(j+1);
            }

        }
    }

    for(int i =0; i<my_points.size(); i++){
        my_points[i].setCurve(my_points[i].getCoord());
    }
}

void To2d(vector<Point>& my_points){
    Filter(my_points);
    Snapping(my_points);
    Padding(my_points);
}

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

    //To2d(all_points);
    //To2d(q_points);

    // lsh l(k,L,2*dim,size);
    // l.lsh_tables_create(all_points);
    // l.query_search(q_points, all_points, out_file);

    // ofstream range_out; 
    // string range_s = "range_search";
    // range_out.open(range_s);

    // l.query_range_search(q_points, all_points, R, range_out);

    // if(N > 1){
    //     ofstream N_near_out; 
    //     string N_near_file = "N_nearest";
    //     N_near_out.open(N_near_file);
    //     l.query_search_N(q_points, all_points, N, N_near_out);
    //     N_near_out.close();
    // }

    // vector<vector<double>> c = all_points[0].getCurve();
    // vector<vector<double>> d = q_points[0].getCurve();
    // cout<<DiscreteFrechet(c,d);
    //To2d(all_points);
    cout<<endl;
    all_points[0].print_point();
    cout<<all_points[0].getDimensions()<<endl;
    cout<<endl;

    To2d(all_points);
    all_points[0].print_point();
    cout<<all_points[0].getDimensions()<<endl;
    cout<<endl;

    close_files(in_file, q_file, out_file);
    // range_out.close();
    
    return 0;
}