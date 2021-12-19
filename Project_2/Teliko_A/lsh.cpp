#include "lsh.h"

/*-------------------------g_hash--------------------------*/

g_hash::g_hash(int k,int dim){
    this->k = k;
    for (int i=0; i<k; i++){
        h_hash h(dim);
        this->hashTable.push_back(h);
        this->r.push_back(random_r());
    }
}

long long int g_hash::g_function(Point& p, int n, int pos){

    long long int sum = 0;

    // Get the random combination of hi’s

    for(int i=0; i<this->k; i++){
        //sum += this->r[i] * this->hashTable[i].hash_function(p);
        sum += modulo((this->r[i] * this->hashTable[i].hash_function(p)), M);
    }
    //sum = modulo(sum, M);

    // Store ID(p) to point
    p.set_ID(sum, pos);

    // Calculate g(p)
    sum = modulo(sum, n/8);

    return sum;
}

/*-------------------------lsh_hash--------------------------*/

lsh::lsh(int k, int L, int dim, int size, int metric){
    this->k = k;
    this->L = L;
    this->dim = dim;
    this->size = size;
    this->metric = metric;


    //Create and store L g
    for(int j=0; j<L; j++){
        g_hash g(k,dim);
        
        this->g_tables.push_back(g);
    }   

    // resize vector to needed size 
    this->G_Hash_Tables.resize(L,vector <vector <Point>> (size/8, vector <Point> ()));

}

void lsh::lsh_tables_create(vector<Point>& all_points){

    // hash all points to the different g and store them to the table
    for(int i=0; i<all_points.size(); i++){
        for(int j=0; j<L; j++){

            long long int g_bucket = g_tables[j].g_function(all_points[i], size, j);

            G_Hash_Tables[j][g_bucket].push_back(all_points[i]);
        }
    }

}

long long int lsh::hash_query(Point& query_point, int g_func){
    long long int q_sum;

    q_sum = g_tables[g_func].g_function(query_point, size, g_func);

    return q_sum;
}

void lsh::query_search(vector<Point> query_points, vector<Point> input_points, ofstream& out_file){
    long long int q_sum;
    double min_dist;
    min_dist = DBL_MAX;
    int id;

    double lsh_dist = 0.0;
    double true_dist = 0.0;

    long double lsh_sum = 0.0;
    long double true_sum = 0.0;

    vector<double> maf;

    for(int i=0; i<query_points.size(); i++){

        auto start1 = chrono::high_resolution_clock::now();

        for(int j=0; j<L; j++){
            // hash the query 
            q_sum = hash_query(query_points[i], j);

            // find the nearest point within the bucket
            for(int z=0; z<G_Hash_Tables[j][q_sum].size(); z++){

                Point temp_p = G_Hash_Tables[j][q_sum][z];
                // calculate the metric distance
                long double d = Distance(temp_p, query_points[i], metric);

                // check if distance is nearest
                if ( d < min_dist){
                    min_dist = d;
                    id = temp_p.getID();
                }
                
            }
        }

        auto stop1 = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed1 = stop1 - start1;
        out_file<<"-------------------------"<<endl;
        out_file<<"Query: "<<query_points[i].getID()<<endl;
        out_file<<"Nearest neighbor-1: "<<id<<endl;
        out_file<<"DistanceLSH:: "<<min_dist<<endl;
        out_file<<"Time of LSH is: "<<elapsed1.count()<<endl;
        
        lsh_dist = min_dist;
        lsh_sum += elapsed1.count();
        
        auto start2 = chrono::high_resolution_clock::now();
        true_dist = brute_force(query_points[i], input_points, out_file);
        auto stop2 = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed2 = stop2 - start2;
        out_file<<"True Time is: "<<elapsed2.count()<<endl;

        true_sum += elapsed2.count();
        
        maf.push_back(lsh_dist/true_dist);

        min_dist = DBL_MAX;
    }

    double tApproximateAverage = lsh_sum/query_points.size();
    double tTrueAverage = true_sum/query_points.size();

    out_file<<"-------------------------"<<endl;
    out_file<<"tApproximateAverage: "<<tApproximateAverage<<endl;
    out_file<<"tTrueAverage: "<<tTrueAverage<<endl;
    out_file<<"MAF: "<<(*max_element(maf.begin(), maf.end()))<<endl;

}

double lsh::brute_force(Point query_point, vector<Point> input_points, ofstream& out_file){
    double min_dist = DBL_MAX;
    long double d;
    int id;

    for (int i=0; i<input_points.size(); i++){
        d = Distance(query_point, input_points[i], metric);
        if(d<min_dist){
            min_dist = d;
            id = input_points[i].getID();
        }
    }

    out_file<<"DistanceTrue: "<<min_dist<<endl;
    out_file<<"True ID of point is: "<<id<<endl;

    return min_dist;
} 


void Print_N_near(vector<N_near> &v){
	
	for(int i=0;i<v.size();++i){
		//cout<<v[i].dis<<' '<<v[i].dis<<endl;
        cout<<v[i].dis<<' '<<endl;
	}
	cout<<endl;
	
}


void lsh::query_search_N(vector<Point> query_points, vector<Point> input_points,int N, ofstream& out_file){
    long long int q_sum;
    int id;

    double min_dist = DBL_MAX;;
    double lsh_sum = 0;
    double true_sum = 0;

    vector<N_near> list_N;
    vector<Point> p_n;

    //Elenxw twn ID twn points
    for(int i=0; i<query_points.size(); i++){
        int p;
        int sum_buckets=0;
        p_n.clear();
        list_N.clear();

        for(int j=0; j<L; j++){
            q_sum = hash_query(query_points[i], j);
            sum_buckets= sum_buckets + G_Hash_Tables[j][q_sum].size();

            //Exoume to q_sum gia na jeroume se poio bucket fa pame
            for(int z=0; z<G_Hash_Tables[j][q_sum].size(); z++){

                Point temp_p = G_Hash_Tables[j][q_sum][z];

                if ( (query_points[i].get_ID(j)) == temp_p.get_ID(j) ) {

                    /*
                    cout<<"To query exei ID: "<<query_points[i].get_ID(j)<<"To id tou query einai:"
                    <<query_points[i].getID()<<endl;
                    cout<<"To point exei ID: "<<temp_p.get_ID(j)<<"To id tou erwthmas einai:"
                    <<temp_p.getID()<<endl;
                    cin>>p;
                    */

                    double d = Distance(temp_p, query_points[i], metric);
                    p_n.push_back(temp_p);
                    list_N.push_back({temp_p,d});
                }
            }
        }

        if ( p_n.size() < N ) //fa xrhshmopoihsoume brute force klasika se ola ta sxetika buckets
        {
            list_N.clear();

            for(int j=0; j<L; j++){

                q_sum = hash_query(query_points[i], j);

                for(int z=0; z<G_Hash_Tables[j][q_sum].size(); z++){

                    Point temp_p = G_Hash_Tables[j][q_sum][z];

                    double d = Distance(temp_p, query_points[i], metric);

                    list_N.push_back({temp_p,d});

                }


            }

            sort(list_N.begin(), list_N.end(), Compare);

            int count = 1;
            int n_print;

            if (list_N.size() < N)
            {
                n_print = list_N.size();
            }   
            else if(list_N.size() >= N)
            {
                n_print = N;
            }

            out_file<<"--------------------------------"<<endl; 
            out_file<<"Query: "<<query_points[i].getID()<<endl;
            
            for (int t = 0 ; t <n_print; t++)
            {
                N_near temp_near = list_N[t];
                out_file<<"Nearest neighbor -"<<count
                        <<": "<<temp_near.p.getID()
                        <<" Distance: "<<temp_near.dis<<endl;
                count++;
            }

            //cout<<"---------------------------"<<endl;
            //cout<<"Gia to query"<<query_points[i].getID()<<endl;
            //cout<<"Exei "<<p_n.size()<<" Shmeia me idio ID"<<endl;
            //cout<<"Kai sunolika se ola ta buckets uparxoun tosa shmeia "<<sum_buckets<<endl;
            //cout<<"Gia to query uparxoyn tosh geitones me apostaseis"<<endl;
            //Print_N_near(list_N);

        }
        else if ( p_n.size() > N) //fa xrhsi brute force mono se auta me to idio ID
        {
            sort(list_N.begin(), list_N.end(), Compare);

            int count=1;
            int n_print;

            if (list_N.size() < N)
            {
                n_print = list_N.size();
            }   
            else if(list_N.size() >= N)
            {
                n_print = N;
            }

            out_file<<"--------------------------------"<<endl;
            out_file<<"Query: "<<query_points[i].getID()<<endl;

            for (int t = 0 ; t <n_print; t++)
            {
                N_near temp_near = list_N[t];
                out_file<<"Nearest neighbor-"<<count
                        <<": "<<temp_near.p.getID()
                        <<" Distance: "<<temp_near.dis<<endl;
                count++;
            }
            
        }
        else //Einai isa aplws ta epistrefw
        {
            sort(list_N.begin(), list_N.end(), Compare);

            int count=1;
            int n_print;

            if (list_N.size() < N)
            {
                n_print = list_N.size();
            }   
            else if(list_N.size() >= N)
            {
                n_print = N;
            }

            out_file<<"--------------------------------"<<endl;
            out_file<<"Query: "<<query_points[i].getID()<<endl;
            for (int t = 0 ; t <n_print; t++)
            {
                N_near temp_near = list_N[t];
                out_file<<"Nearest neighbor -"<<count
                        <<": "<<temp_near.p.getID()
                        <<" Distance: "<<temp_near.dis<<endl;
                count++;
            }
        }
    }
}

void lsh::query_range_search(vector<Point> query_points, vector<Point> input_points, double R, ofstream& out_file){
    long long int q_sum;
    double min_dist;
    min_dist = DBL_MAX;
    int id;
    double lsh_sum = 0;
    double true_sum = 0;

    vector<N_near> list_N;
    vector<Point> p_n;

    //Elenxw twn ID twn points
    for(int i=0; i<query_points.size(); i++){

        int p;
        int sum_buckets=0;
        p_n.clear();
        list_N.clear();

        for(int j=0; j<L; j++){
            
            q_sum = hash_query(query_points[i], j);
            sum_buckets= sum_buckets + G_Hash_Tables[j][q_sum].size();
            //Exoume to q_sum gia na jeroume se poio bucket fa pame
            for(int z=0; z<G_Hash_Tables[j][q_sum].size(); z++){

                Point temp_p = G_Hash_Tables[j][q_sum][z];
                double d = Distance(temp_p, query_points[i], metric);

                if ( d< R )
                {
                    list_N.push_back({temp_p,d});
                }
            }
        }

        sort(list_N.begin(), list_N.end(), Compare);
        out_file<<"------------------------------\n";
        out_file<<"Query: "<<query_points[i].getID()<<endl;
        out_file<<"R-near neighbors: "<<endl;

        for (int t = 0 ; t <list_N.size(); t++)
        {          
            N_near temp_near = list_N[t];
            out_file<<"Neighbor "<<t<<" id: "<<temp_near.p.getID()
                    <<" Distance: "<<temp_near.dis<<endl;
        }

    }
}

Apotelesma_range lsh::query_range_search_clu(Point query_points,double R){
    long long int q_sum;
    double min_dist;
    min_dist = DBL_MAX;
    int id;
    double lsh_sum = 0;
    double true_sum = 0;

    vector<N_near> list_N;
    vector<Point> p_n;
    Apotelesma_range ap;

        auto start1 = chrono::high_resolution_clock::now();
        int p;
        int sum_buckets=0;
        p_n.clear();
        list_N.clear();

        
        //query_points.print_point();

        for(int j=0; j<L; j++){
            
            q_sum = hash_query(query_points, j);

            sum_buckets= sum_buckets + G_Hash_Tables[j][q_sum].size();
            //Exoume to q_sum gia na jeroume se poio bucket fa pame
            for(int z=0; z<G_Hash_Tables[j][q_sum].size(); z++){

                

                Point temp_p = G_Hash_Tables[j][q_sum][z];
                if(temp_p.getCluster_ID() == query_points.getCluster_ID())
                {
                    continue;
                }

                double d = euc_dist(temp_p.getCoord(), query_points.getCoord());

                

                if ( sqrt(d) < R )
                {           
                    //list_N.push_back({temp_p,d});
                    if(temp_p.getCluster_ID() == 0)
                    {
                        ap.kanonika_apot.push_back(temp_p.getID());
                    }
                    else if( temp_p.getCluster_ID() != query_points.getCluster_ID())
                    {
                        ap.bloblhmatika_apot.push_back(temp_p.getID());
                        ap.bloblh_kentro.push_back(temp_p.getCluster_ID());
                    }
                }
            }
        }

        
    
    return ap;
}