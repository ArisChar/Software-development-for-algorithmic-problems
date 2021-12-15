#include "help_func.h"

long long int modulo(long long int a, long long int b){

	return (a % b + b) %b;
}

long double euc_dist(vector<double > in_vec, vector<double> q_vec){
    long double  dist = 0;

    for(int i=0; i<in_vec.size(); i++){
        dist = dist + pow(in_vec[i]-q_vec[i],2);
    }

    return dist;
}

bool Compare(N_near a, N_near b){
	return a.dis < b.dis;
}

//--------------input_handling--------------------

void input_handling(int argc, char *argv[], string& input_file, string& query_file, string& output_file, int& k, int& L, int& N, int& R){

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

    while ((c = getopt (argc, argv, ":i:q:k:L:o:N:R:")) != -1){
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
            case 'R':
                if(optarg[0] == '-'){
                    cout<<"Option -R requires an numbered argument\n";
                    exit(EXIT_FAILURE);
                }else{
                    R = atoi(optarg);
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
		if(line[i]== ' ') {
            dim++;
        }
	}

    input_file.clear();
    input_file.seekg(0);

    return dim-1;
}