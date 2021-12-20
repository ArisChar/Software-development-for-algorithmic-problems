#include "help_func.h"
#include "random.h"




double DiscreteFrechet(vector<vector<double>> a, vector<vector<double>> b){
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

    return c[a.size()-1][b.size()-1];
}

long long int modulo(long long int a, long long int b){

	return (a % b + b) %b;
}

long double euc_dist(vector<double> in_vec, vector<double> q_vec){
    long double  dist = 0;

    for(int i=0; i<in_vec.size(); i++){
        dist = dist + pow(in_vec[i]-q_vec[i],2);
    }

    return dist;
}

bool Compare(N_near a, N_near b){
	return a.dis < b.dis;
}

vector<vector<double>>  mean_curve(vector<vector<double>> a, vector<vector<double>> b){
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

    /*for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
        {
            cout<<c[i][j]<<" ";
        }
        
        cout<<endl;
    }*/

    vector<vector<int>> diafromh;

    int i = a.size() - 1;
    int j = b.size() - 1;
    diafromh.push_back({i,j});
    bool epan = true;

    while (epan)
    {
        

        if(i>0 && j>0)
        {
            if (c[i-1][j-1] <= c[i][j-1])
            {
                if(c[i-1][j-1] <= c[i-1][j] )
                {
                    i--;
                    j--;
                    diafromh.push_back({i,j});
                }
                else if (c[i-1][j-1] > c[i-1][j])
                {
                    i--;
                    diafromh.push_back({i,j});
                }            
            }
            else if (c[i-1][j-1] > c[i][j-1])
            {
                if(c[i][j-1] <= c[i-1][j])
                {
                    j--;
                    diafromh.push_back({i,j});
                }
                else if (c[i][j-1] > c[i-1][j])
                {
                    i--;
                    diafromh.push_back({i,j});
                }               
            }              
        }

        if(i>0 && j==0)
        {
            i--;
            diafromh.push_back({i,j});
        }

        if(i==0 && j>0)
        {
            j--;
            diafromh.push_back({i,j});
        }


        if(i == 0 && j ==0)
        {
            epan = false;
        }
        
    }
    
    /*for (int i = 0; i < diafromh.size(); i++)
    {
        for (int j = 0; j < diafromh[i].size(); j++)
        {
            cout<<diafromh[i][j]<<" ";
        }
        
        cout<<endl;
    }*/

    vector<vector<double>> mean_c;

    double xm;
    double ym;
    
    double x1;
    double x2;

    double y1;
    double y2;

    for (int i = 0; i < diafromh.size(); i++)
    {
        //xm = diafromh[i][0];
        //ym = diafromh[i][1];
        x1 = a[diafromh[i][0]][0];
        //cout<<"x1: "<<x1<<" ";
        y1 = a[diafromh[i][0]][1];
        //cout<<"y1: "<<y1<<" ";
        
        x2 = b[diafromh[i][1]][0];
        //cout<<"x2: "<<x2<<" ";
        y2 = b[diafromh[i][1]][1];
        //cout<<"y2: "<<y2<<" ";
        //cout<<endl;
        xm =(x1+x2)/2 ;
        ym =(y1+y2)/2 ;

        mean_c.push_back({xm,ym});
    }
    
    reverse( mean_c.begin(), mean_c.end() );

    return mean_c;
}

vector<vector<double>>  mean_curve_many(vector<vector<vector<double>>> all_curve)
{

    int count_all = all_curve.size();

    //int number_log = log2(count_all);
    //int number_floor = floor(number_log);  
    //int number_epan = number_floor * 2;
    //int number_rest  = count_all - number_epan;

    int number_log = count_all/2;
    int number_epan = number_log * 2;
    int number_rest = count_all%2;

    bool epan = true;

    vector<vector<vector<double>>> my_tree_curve = all_curve;
    vector<vector<vector<double>>> new_tree_curve ;
    while(epan)
    {
        for (int i = 0; i < number_epan; i+=2)
        {
            //cout<<"number_floor: "<<number_floor<<endl;

            /*cout<<"count_all: "<<count_all<<endl;
            cout<<"number_log: "<<number_log<<endl; 
            cout<<"number_epan: "<<number_epan<<endl;
            cout<<"number_rest: "<<number_rest<<endl;

            cout<<"Kampulh noumero: "<<i<<endl;*/

            /*for (int z = 0; z < my_tree_curve[i].size(); z++)
            {
                for (int j = 0; j < my_tree_curve[i][z].size(); j++)
                {
                    cout<<my_tree_curve[i][z][j]<<" ";
                }
        
                cout<<endl;
            }
            cout<<"Kampulh noumero: "<<i+1<<endl;
            for (int z = 0; z < my_tree_curve[i+1].size(); z++)
            {
                for (int j = 0; j < my_tree_curve[i+1][z].size(); j++)
                {
                    cout<<my_tree_curve[i+1][z][j]<<" ";
                }
        
                cout<<endl;
            }*/


            new_tree_curve.push_back(mean_curve(my_tree_curve[i],my_tree_curve[i+1]));
            //int tt;
            //cin>>tt;
        }
        if(number_rest > 0)
        {
            //cout<<"Ypoloipes kampules"<<endl;
            
            for (int i = number_epan; i < count_all; i++)
            {
                //int tt;
                //cin>>tt;
                //cout<<"Kampulh noumero: "<<i<<endl;
                /*for (int z = 0; z < my_tree_curve[i].size(); z++)
                {
                    for (int j = 0; j < my_tree_curve[i][z].size(); j++)
                    {
                        cout<<my_tree_curve[i][z][j]<<" ";
                    }
        
                    cout<<endl;
                }*/


                new_tree_curve.push_back(my_tree_curve[i]);
            }
        }
        

        if(count_all > 1 )
        {
            my_tree_curve = new_tree_curve ;
            new_tree_curve.clear();

            count_all = my_tree_curve.size();

            number_log = count_all / 2;
            number_epan = number_log * 2;
            number_rest =  count_all % 2;

            //number_log = log2(count_all);
            //number_floor = floor(number_log);    
            //number_epan = number_floor * 2;
            //number_rest  = count_all - number_epan;
        }

        if (count_all == 1)
        {
            epan = false;
        }
        

    }
    
    return my_tree_curve[0];
    
}

//--------------input_handling--------------------

void input_handling(int argc, char *argv[], string& input_file, string& query_file, string& output_file, int& a, int& C, int& update){

    if (argc < 7){
        cout<<"Error not enough command line arguments\n";
        exit(EXIT_FAILURE);
    }  

    if(strcmp(argv[1],"-i") != 0 || strcmp(argv[3],"-c") != 0){
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

    while ((c = getopt (argc, argv, ":i:c:k:a:o:C:u:")) != -1){
        switch (c) {
            case 'i':
                input_file = optarg;
                break;
            case 'c':
                query_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;  
            case 'a':
                if(optarg[0] == '-'){
                    cout<<"Option -a requires an int argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    if(strcmp(optarg,"Classic") == 0){
                        a = 0;
                        break;
                    }else if(strcmp(optarg,"LSH") == 0){
                        a = 1;
                        break;
                    }else if(strcmp(optarg,"Hypercube") == 0){
                        a = 2;
                        break;
                    }else if(strcmp(optarg,"LSH_Frechet") == 0){
                        a = 1;
                        break;
                    }else{
                        cout<<"Option -a requires Classic OR LSH or Hypercube or LSH_Frechet\n";
                        exit(EXIT_FAILURE);
                    }
                }
            case 'C':
                if(optarg[0] == '-'){
                    cout<<"Option -C requires yes or no argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    if(strcmp(optarg,"yes") == 0){
                        C = 1;
                        break;
                    }else if(strcmp(optarg,"no") == 0){
                        C = 0;
                        break;
                    }else{
                        cout<<"Option -C requires yes or no\n";
                        exit(EXIT_FAILURE);
                    }
                }
            case 'u':
                if(optarg[0] == '-'){
                    cout<<"Option -u requires Frechet or Vector argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    if(strcmp(optarg,"Vector") == 0){
                        update = 0;
                        break;
                    }else if(strcmp(optarg,"Frechet") == 0){
                        update = 1;
                        break;
                    }else{
                        cout<<"Option -u requires Frechet or Vector argument\n";
                        exit(EXIT_FAILURE);
                    }
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

void read_conf(ifstream& c_file, string conf_file, int& K, int& L, int& k, int& M_hype, int& k_hype, int& probes){

    string line;
    vector<int> values;

    while( getline(c_file, line) )
    {
        istringstream is_line(line);
        string key;
        if( getline(is_line, key, ':') )
        {
            string value;
            int v;
            if(getline(is_line, value)) {
                v = atoi(value.c_str());
                values.push_back(v);
            }
        }
    }
    
    K = values[0];
    L = values[1];
    k = values[2];
    M_hype = values[3];
    k_hype = values[4];
    probes = values[5];

}

void To2d(vector<Point>& my_points){
    Snapping(my_points);
    Padding(my_points);
}

void Snapping(vector<Point>& my_points){
    int d = 2;
    double tx = uniform_distr(d);
    double ty = uniform_distr(d);

    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j++){

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

void Padding(vector<Point>& my_points){
    int size = my_points[0].getCoord().size();

    vector<double> target;
    double max_x = 10*size;
    double max_y = 100002;
    
    for(int i=0; i<my_points.size(); i++){

        for(int j=0; j<my_points[i].getCoord().size(); j+=2){   

            if(target.size()==0 || my_points[i].getVal(j+1) != target.end()[-1] || my_points[i].getVal(j) != target.end()[-2]){
                target.push_back(my_points[i].getVal(j));
                target.push_back(my_points[i].getVal(j+1));
            }

        }

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

