#include "help_func.h"
#include "random.h"

long double Distance(Point in, Point query, int metric){
    long double dist;

    if(metric == 0){ // L2

        //cout<<"Calculating distance with L2\n";
        dist = sqrt(euc_dist(in.getCoord(), query.getCoord()));
    }
    else if(metric == 1){ // Discrete Frechet

        //cout<<"Calculating distance with Discrete\n";
        dist = sqrt(DiscreteFrechet(in.getCurve(), query.getCurve()));
    }
    else if(metric ==2){ // Continuous Frechet

        //cout<<"Calculating distance with Continuous\n";
        dist = ContinuousFrechet(in.getFiltered(), query.getFiltered());
    }

    return dist;
}

void Snapping(vector<Point>& my_points, double d){
    double ty = uniform_distr(d); // random t for 

    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j++){

            double y = my_points[i].getVal(j);
            // ypologismos neou y 
            double new_y = (floor(((y+ty)/d))*d)-d;
            my_points[i].set_value(new_y,j); 

        }
    }
}

void Padding(vector<Point>& my_points){
    vector<double> target;

    double max_y = 10000.0;
    double minima;
    double maxima;
    double x1,x2,x3;
    vector<double> new_points;
    //cout<<"dim is"<<my_points[0].getDimensions()<<endl;

    for(int i=0; i<my_points.size(); i++){

        //cout<<"Sinolika points prin "<<my_points[i].getCoord().size()<<endl;
        // check every e elemnts and check minima and maxima
        for(int j=0; j<my_points[i].getCoord().size(); j++){ 
            x1 = my_points[i].getVal(j);
            x2 = my_points[i].getVal(j+1);
            x3 = my_points[i].getVal(j+2);   

            minima = min(x1,x3);
            maxima = max(x1,x3);

            if( minima <= x2 && x2 <= maxima){
                // push the 1st element and skip the next one (go to the 3rd)
                target.push_back(x1);
                if(my_points.size()-3 == j){
                    // if we are at the last 3 also push 3rd
                    target.push_back(x3);
                    break;
                }
                j++;
            }else{
                // push 1st
                target.push_back(x1);
                if(my_points.size()-3 == j){
                    // if we are at the end also push 2nd and 3rd
                    target.push_back(x2);
                    target.push_back(x3);
                    break;
                }
            }

        }

        for(int j=0; j<my_points[i].getDimensions(); j++){  

            if( j < target.size() ){
                new_points.push_back(target[j]);
            }else{
                new_points.push_back(max_y);
            }

        }
        my_points[i].replace(new_points);
        //cout<<"Sinolika points meta "<<my_points[i].getCoord().size()<<endl<<endl;
        target.clear();
        new_points.clear();
    }

}

vector<Point> Filter(vector<Point>& my_points){
    double e = 0.15;
    double x1,x2,x3;
    // filter the data by checking every 3 elements
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
        my_points[i].setCurve3(my_points[i].getCoord());
    }

    return my_points;
}

void PutFiltered(vector<Point>& target_points, vector<Point>& filtered_points){
    vector<double> values;
    // store filtered values to Point
    for(int i=0; i< target_points.size(); i++){
        values = filtered_points[i].getCoord();
        target_points[i].setFiltered(values);
    }
}

void To2d(vector<Point>& my_points, double delta){
    vector<Point> filtered_points;
    filtered_points = Filter(my_points);
    Snapping(my_points, delta);
    Padding(my_points);
    PutFiltered(my_points, filtered_points);
}

void SnappingDiscrete(vector<Point>& my_points, double d){
    double tx = uniform_distr(d); // random t for x
    double ty = uniform_distr(d); // random t for y

    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j++){
            // We want to snapp only every even value
            if ( j % 2 == 0){
                double x = my_points[i].getVal(j);
                double new_x = floor(((x-tx)/d) + 0.5)*d + tx;
                my_points[i].set_value(new_x,j); 
            }else{
                double y = my_points[i].getVal(j);
                double new_y = floor(((y-ty)/d) + 0.5)*d + ty;
                my_points[i].set_value(new_y,j); 
            }

        }

    }

}

void PaddingDiscrete(vector<Point>& my_points){
    int size = my_points[0].getCoord().size();
    // temporary vector to sotre values that stay from the removal of consecutive duplicates 
    vector<double> target;
    double max_x = 10*size;
    double max_y = 100002;
    
    for(int i=0; i<my_points.size(); i++){
        // Remove all consecutive duplicates 
        for(int j=0; j<my_points[i].getCoord().size(); j+=2){   

            if(target.size()==0 || my_points[i].getVal(j+1) != target.end()[-1] || my_points[i].getVal(j) != target.end()[-2]){
                target.push_back(my_points[i].getVal(j));
                target.push_back(my_points[i].getVal(j+1));
            }

        }
        // move the new values from target back to my_points and fill the rest with big values
        for(int j=0; j<my_points[i].getCoord().size(); j+=2){  

            if( j < target.size() ){
                my_points[i].set_value(target[j],j);
                my_points[i].set_value(target[j+1],j+1);
            }else{
                my_points[i].set_value(max_x,j);
                my_points[i].set_value(max_y,j+1);
            }

        }

        target.clear();
    }
}

void To2dDiscrete(vector<Point>& my_points, double delta){
    SnappingDiscrete(my_points, delta);
    PaddingDiscrete(my_points);
}

long double ContinuousFrechet(vector<double> in_vec, vector<double> q_vec){
    int dims = 1;
    double fred_dist;
    // cout<<"Fred\n";
    //transform our data types to fred data types to calculate ContinuousFrechet distance

    int in_num_of_points = in_vec.size();
    // cout<<"in_num_of_points "<<in_num_of_points<<endl;

    Curve in_curve(in_num_of_points,dims);

    for(int j=0; j<in_vec.size(); j++){
        double val = in_vec[j];
        FredPoint temp(dims);
        temp.set(0,val);
        in_curve.push_back(temp);
    }

    int q_num_of_points = q_vec.size();
    // cout<<"q_num_of_points "<<q_num_of_points<<endl;
    Curve q_curve(q_num_of_points,dims);
    
    for(int i=0; i<q_vec.size(); i++){
        double val = q_vec[i];
        FredPoint temp(dims);
        temp.set(0,val);
        q_curve.push_back(temp);
    }

    fred_dist = Frechet::Continuous::distance(in_curve,q_curve).value;

    // cout<<"Fred dist is :"<<fred_dist<<endl<<endl;

    return fred_dist;
}

long double DiscreteFrechet(vector<vector<double>> a, vector<vector<double>> b){
    double fred_dist;

    vector<vector<double>> dists;
    dists.resize(a.size(), vector<double>((b.size())));

    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            dists[i][j] = euc_dist(a[i],b[j]); 
        }
    }

    vector<vector<double>> c;
    c.resize(a.size(), vector<double>((b.size())));

    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            if(i == 0 && j == 0){
                c[i][j] = dists[i][j];
            }else if (i == 0 && j > 0){
                c[i][j] = max(c[i][j-1],dists[i][j]);
            }else if (i > 0 and j == 0){
                c[i][j] = max(c[i-1][j],dists[i][j]);
            }else{
                c[i][j] = max(min(min(c[i-1][j], c[i-1][j-1]), c[i][j-1]), dists[i][j]);
            }
        }
    }    

    fred_dist = c[a.size()-1][b.size()-1];

    return fred_dist;
}

long double euc_dist(vector<double> in_vec, vector<double> q_vec){
    long double  dist = 0;

    for(int i=0; i<in_vec.size(); i++){
        dist = dist + pow(in_vec[i]-q_vec[i],2);
    }

    return dist;
}

long long int modulo(long long int a, long long int b){

	return (a % b + b) %b;
}

bool Compare(N_near a, N_near b){
	return a.dis < b.dis;
}

//--------------input_handling--------------------

void input_handling(int argc, char *argv[], string& input_file, string& query_file, string& output_file, int& k, int& L, int& N, int& M, int& probes, int& algor, int& metric, double& d, int& K){

    if (argc < 7){
        cout<<"Error not enough command line arguments\n";
        exit(EXIT_FAILURE);
    }  

    if(strcmp(argv[1],"-i") != 0 || strcmp(argv[3],"-q") != 0){
        cout<<"Error did not specify files"<<endl;
        exit(EXIT_FAILURE);
    }

    int i = 1;
    int flag = 0;

    while( i < argc ){
        if(strcmp(argv[i],"-o") == 0){
            flag = 1;
        }
        i++;
    }

    if(flag == 0){
        cout<<"Error no output file found"<<endl;
        exit(EXIT_FAILURE);
    }

    int c;
    opterr = 0;

    while ((c = getopt (argc, argv, ":i:q:o:k:L:N:m:p:a:m:d:K:M")) != -1){
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
            case 'q':
                query_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;  
            case 'k':
                if(optarg[0] == '-'){
                    cout<<"Option -k requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    k = atoi(optarg);
                    break;
                }
            case 'L':
                if(optarg[0] == '-'){
                    cout<<"Option -L requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    L = atoi(optarg);
                    break;
                }
            case 'N':
                if(optarg[0] == '-'){
                    cout<<"Option -N requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    N = atoi(optarg);
                    break;
                }
            case 'M':
                if(optarg[0] == '-'){
                    cout<<"Option -M requires an numbered argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    M = atoi(optarg);
                    break;
                }
            case 'p':
                if(optarg[0] == '-'){
                    cout<<"Option -probes requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    probes = atoi(optarg);
                    break;
                }
            case 'a':{ // algorithm
                algor = atoi(optarg);
                if(algor>= 0 && algor <= 2 ){
                    break;
                }else{
                    cout<<"Option -a requires number from [0,2]\n";
                    cout<<"0 for LSH\n"
                        <<"1 for Hypercube\n"
                        <<"2 for Frechet\n";
                    exit(EXIT_FAILURE);
                }
                
            }
            case 'm':{ //metric
                metric = atoi(optarg);
                if(metric>= 1 && metric <= 2 ){
                    break;
                }else{
                    cout<<"Option -m requires number from [1,2]\n"
                        <<" 1 for discrete\n 2 for continuous \n";
                    exit(EXIT_FAILURE);
                }
            }
            case 'd':{ // delta
                d = atoi(optarg);
                break;
            }
            case 'K':
                if(optarg[0] == '-'){
                    cout<<"Option -K requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    K = atoi(optarg);
                    break;
                }            
            case ':':
                cout<<"Argument(s) without parameter found"<<endl;
                exit(EXIT_FAILURE);
            default:
                cout<<"Error wrong argument(s) found"<<endl;
                exit(EXIT_FAILURE);
        }
    }
}

int open_files(ifstream& in_file, ifstream& q_file, ofstream& out_file, string& input_file, string& query_file, string& output_file){
    
    in_file.open(input_file);
    if(!in_file){
		cout << "Didn't find input file" << endl;
	    return -1;
    }

    q_file.open(query_file);
    if(!q_file){
		cout << "Didn't find query file" << endl;
	    return -1;
    }

    out_file.open(output_file);
    if(!out_file){
		cout << "Could not open output file" << endl;
	    return -1;
    }

    return 0;
}

void close_files(ifstream& in_file, ifstream& q_file, ofstream& out_file){
    in_file.close();
    q_file.close();
    out_file.close();
}

int get_input_size(ifstream& input_file){
    int size = 0;
    string line;

    if(input_file.is_open()){
        while(getline(input_file, line)){
            size++;
        }
    }

    input_file.clear();
    input_file.seekg(0);

    return size;
}

int get_dim(ifstream& input_file){
    int dim = 0;
    string line;

    getline(input_file,line);

    for(int i = 0; i<line.size(); i++) {
		if(line[i]== '	') {
            dim++;
        }
	}

    input_file.clear();
    input_file.seekg(0);

    return dim;
}