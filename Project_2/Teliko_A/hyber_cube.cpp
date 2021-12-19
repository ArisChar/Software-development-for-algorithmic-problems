#include "hyber_cube.h"
#include "help_func.h"

//---------------------f_hash-----------------------//

f_hash::f_hash(int dim){
    h_hash h_f(dim);
    h.push_back(h_f);
}

int f_hash::f_function(Point p){

    long long int x = h[0].hash_function(p);
    
    int r = rand()%2;

    f_pair.insert({x,r});

    return f_pair[x];
}

unsigned concat(unsigned x, unsigned y) {
    unsigned pow = 10;

    while(y >= pow){
        pow *= 10;
    }
        
    return x * pow + y;        
}

hypercube::hypercube(int k, int dim){
    this->k = k;

    for(int i=0; i<this->k; i++){
        f_hash f(dim);
        f_number.push_back(f);
    }

    int d = pow(2,k);

    hypercube_table.resize(d,vector<Point>());
}

void hypercube::hypercube_input(vector<Point> all_points){
    
    for(int i=0; i<all_points.size(); i++){
        unsigned long int index = 0;
        string bit = "";
        
        
        for(int j=0; j<k; j++){
            string temp;

            int bit_f = f_number[j].f_function(all_points[i]); //returns 0 or 1

            temp = to_string(bit_f);

            bit = bit + temp;
        }

        index = stoi(bit, 0, 2);
        hypercube_table[index].push_back(all_points[i]);
    }     
}

void hypercube::print_hyper(){

    for(int i=0; i<hypercube_table.size(); i++) {
        for(int j=0; j<hypercube_table[i].size(); j++){
            cout<<hypercube_table[i][j].getID()<<"||";
        }
        cout<<endl<<"=================="<<endl;
    }

}

void hypercube::hyper_search(Point query, int N, int m, int probe, ofstream& out_file)
{
    int elenxws;
    
    vector<N_near> list_N;
    unsigned long int index = 0;
    string bit = "";
    int my_m = m;
    int my_probes = probe;

    //To kanw gia to buckets tou query   
    for(int j=0; j<this->k; j++)
    {
        string temp;

        int bit_f = f_number[j].f_function(query); //returns 0 or 1

        temp = to_string(bit_f);

        bit = bit + temp;
    }

    index = stoi(bit, 0, 2);

    //cout<<"Prwto bucket"<<endl;
    //cin>>elenxws;
    for (int z = 0; z < hypercube_table[index].size(); z++)
    {
        Point temp_p = hypercube_table[index][z];
        double d = euc_dist(temp_p.getCoord(), query.getCoord());
                    
        list_N.push_back({temp_p,d});
        my_m--;

        

        if( my_m == 0)
        {   
            //cout<<"Brefhkan ola ta stoixeia sto prwto bucket"<<endl;
            //cin>>elenxws;

            //ekptupwseis
            sort(list_N.begin(), list_N.end(), Compare);
            out_file<<"-------------------------"<<endl;
            out_file<<"Query: "<<query.getID()<<endl;
            int count=0;
            int prin_num;
            if( N < list_N.size())
            {
                prin_num = N;
            }
            else if( N > list_N.size())
            {
                prin_num = list_N.size();
            }

            for (int t = 0 ; t < prin_num; t++)
            {          
                N_near temp_near = list_N[t];
                out_file<<"Nearest neighbor -"<<t+1
                        <<": "<<temp_near.p.getID()<<endl
                        <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                count++;
                
                if( count == m )
                {
                    return;
                }
            }

            return ;
        }

        //my_m--;
        
    }
    //cout<<"Telos prwtou bucket"<<endl;
    //cout<<"Brefhkan mexri twra tosa stoixeia: "<<list_N.size()<<endl;
    //cin>>elenxws;

    my_probes--;

    if( my_probes == 0 )
    {
        //cout<<"2 Brefhkan ola ta stoixeia sto prwto bucket"<<endl;
        //cin>>elenxws;
        //ekptupwseis
        sort(list_N.begin(), list_N.end(), Compare);
        out_file<<"--------------------------------"<<endl;
        out_file<<"Query: "<<query.getID()<<endl;
        out_file<<"Ta "<<N<<" pio kontina shmeia einai :"<<endl;
        int count=0;
        int prin_num;
        if( N < list_N.size())
        {
            prin_num = N;
        }
        else if( N > list_N.size())
        {
            prin_num = list_N.size();
        }
        for (int t = 0 ; t < prin_num ; t++)
        {          
            N_near temp_near = list_N[t];
            out_file<<"Nearest neighbor -"<<t+1
                    <<": "<<temp_near.p.getID()<<endl
                    <<"Distance: "<<sqrt(temp_near.dis)<<endl;
            count++;
            if( count == m )
            {
                return;
            }
        }

        return ;
    }

    //Jekinhma diadikasias jafumatos se alla bucket bash hamming distance
    std::vector<char> str;
    for (int i = 0; i < bit.length(); i++)
        {
        	str.push_back(bit[i]);
        	//cout << str[i];
    	}

    bool new_ham = true;
    int len = str.size();
    int maxDistance = this->k;     
            
    while( my_probes > 0 )
    {   
        //Ypologismos twn allwn hamming distance
        
        vector<string> str_re;  
        int index2;
        int index_str;
        string bit2="";
        int ham_ep;
        if( new_ham == true)
        {
            new_ham = false;
            //for (size_t ham = 0; i < this->k; ham++)
            //{
                //cout<<"Ypologismos tou hamming distance"<<"Me maxDistance: "<<maxDistance<<endl;
                //cin>>elenxws;
                str_re.clear();
                for (size_t i = 1 ; i <= maxDistance ; ++i) 
                {
                    //printf("Computing for distance %lu\n", i);
                    hamming_dis_near(str, len-1, i,str_re);
                    //printf("----------------\n");
                }
            //}

            //index_str = rand() % str_re.size();
            
            //bit2 = str_re[index_str];
            //index2 = stoi(bit, 0, 2);
            ham_ep = str_re.size();

        }

        for (int b = 0; b < ham_ep; b++)
        {
            
            bit2 = str_re[b];
            index2 = stoi(bit2, 0, 2);
            //cout<<"mpeneis se kanourgio buckets: "<<index2<<endl;
            //cin>>elenxws;
            my_probes--;

            if(my_probes > 0)
            {

                for (int z = 0; z < hypercube_table[index2].size(); z++)
                {
                    my_m--;
                    Point temp_p = hypercube_table[index2][z];
                    double d = euc_dist(temp_p.getCoord(), query.getCoord());
                    
                    list_N.push_back({temp_p,d});

                    if( my_m == 0)
                    {
                        //cout<<"Teleiwsan ta my_m"<<endl;
                        //cin>>elenxws;
                        //ekptupwseis
                        sort(list_N.begin(), list_N.end(), Compare);
                        out_file<<"--------------------------------"<<endl;
                        out_file<<"Query: "<<query.getID()<<endl;
                        out_file<<"Ta "<<N<<" pio kontina shmeia einai :"<<endl;
                        int count=0;
                        int prin_num;
                        if( N < list_N.size())
                        {
                            prin_num = N;
                        }
                        else if( N > list_N.size())
                        {
                            prin_num = list_N.size();
                        }
                        for (int t = 0 ; t < prin_num ; t++)
                        {          
                            N_near temp_near = list_N[t];
                            out_file<<"Nearest neighbor -"<<t+1
                                    <<": "<<temp_near.p.getID()<<endl
                                    <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                            count++;
                            if( count == m )
                            {
                                return;
                            }
                        }

                        return ;
                    }

                    //my_m--;

                }
            }

            //my_probes--;

            if(my_probes <= 0)
            {
                //cout<<"Teleiwsan ta my_probes"<<endl;
                //cin>>elenxws;
                //ekptupwseis
                sort(list_N.begin(), list_N.end(), Compare);
                out_file<<"--------------------------------"<<endl;
                out_file<<"Query: "<<query.getID()<<endl;
                out_file<<"Ta "<<N<<" pio kontina shmeia einai :"<<endl;
                int count=0;
                int prin_num;
                if( N < list_N.size())
                {
                    prin_num = N;
                }
                else if( N > list_N.size())
                {
                    prin_num = list_N.size();
                }
                for (int t = 0 ; t < prin_num ; t++)
                {          
                    N_near temp_near = list_N[t];
                    out_file<<"Nearest neighbor -"<<t+1
                            <<": "<<temp_near.p.getID()<<endl
                            <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                    count++;
                    if( count == m )
                    {
                        return;
                    }
                }
                return ;
            }
        }
    }
}

void hypercube::hyper_search_N(vector<Point> query, int N, int m, int probe, ofstream& out_file)
{
    // edw xronous
    

    for (int i = 0; i < query.size(); i++)
    {
        auto start = chrono::high_resolution_clock::now();

        hyper_search(query[i],N,m,probe, out_file);

        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = stop - start;

        out_file<<"\n Total time taken: "<<elapsed.count()<<endl<<endl;
    }

}

void hypercube::hyper_search_R(Point query, int R, int m, int probes, ofstream& out_file)
{
    int elenxws;
    
    vector<N_near> list_N;
    unsigned long int index = 0;
    string bit = "";
    int my_m = m;
    int my_probes = probes;

    //To kanw gia to buckets tou query   
    for(int j=0; j<this->k; j++)
    {
        string temp;

        int bit_f = f_number[j].f_function(query); //returns 0 or 1

        temp = to_string(bit_f);

        bit = bit + temp;
    }

    index = stoi(bit, 0, 2);

    //cout<<"Prwto bucket"<<endl;
    //cin>>elenxws;
    for (int z = 0; z < hypercube_table[index].size(); z++)
    {
        Point temp_p = hypercube_table[index][z];
        double d = euc_dist(temp_p.getCoord(), query.getCoord());
                    
        if( R > sqrt(d))
        {
            list_N.push_back({temp_p,d});
        }


        my_m--;

        

        if( my_m == 0)
        {   
            //cout<<"Brefhkan ola ta stoixeia sto prwto bucket"<<endl;
            //cin>>elenxws;

            //ekptupwseis
            sort(list_N.begin(), list_N.end(), Compare);
            out_file<<"--------------------------------"<<endl;
            out_file<<"Query: "<<query.getID()<<endl;
            out_file<<"Nearest point within range R"<<R<<": "<<endl;
            int count=0;
            
            for (int t = 0 ; t < list_N.size(); t++)
            {          
                N_near temp_near = list_N[t];
                out_file<<"Nearest neighbor -"<<t+1
                        <<" id: "<<temp_near.p.getID()<<endl
                        <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                count++;
                
                if( count == m )
                {
                    return;
                }
            }

            return ;
        }  
    }

    my_probes--;

    if( my_probes == 0 )
    {
        sort(list_N.begin(), list_N.end(), Compare);
        out_file<<"--------------------------------"<<endl;
        out_file<<"Query: "<<query.getID()<<endl;
        out_file<<"Nearest point within range R"<<R<<": "<<endl;
        int count=0;
        for (int t = 0 ; t < list_N.size() ; t++)
        {          
            N_near temp_near = list_N[t];
            out_file<<"Nearest neighbor -"<<t+1
                    <<" id: "<<temp_near.p.getID()<<endl
                    <<"Distance: "<<sqrt(temp_near.dis)<<endl;
            count++;
            if( count == m )
            {
                return;
            }
        }

        return ;
    }

    //Jekinhma diadikasias jafumatos se alla bucket bash hamming distance
    std::vector<char> str;
    for (int i = 0; i < bit.length(); i++)
        {
        	str.push_back(bit[i]);
        	//cout << str[i];
    	}

    bool new_ham = true;
    int len = str.size();
    int maxDistance = this->k;     
            
    while( my_probes > 0 )
    {   
        //Ypologismos twn allwn hamming distance
        
        vector<string> str_re;  
        int index2;
        int index_str;
        string bit2="";
        int ham_ep;
        if( new_ham == true)
        {
            new_ham = false;
            //for (size_t ham = 0; i < this->k; ham++)
            //{
                //cout<<"Ypologismos tou hamming distance"<<"Me maxDistance: "<<maxDistance<<endl;
                //cin>>elenxws;
                str_re.clear();
                for (size_t i = 1 ; i <= maxDistance ; ++i) 
                {
                    //printf("Computing for distance %lu\n", i);
                    hamming_dis_near(str, len-1, i,str_re);
                    //printf("----------------\n");
                }
            //}

            //index_str = rand() % str_re.size();
            
            //bit2 = str_re[index_str];
            //index2 = stoi(bit, 0, 2);
            ham_ep = str_re.size();

        }

        for (int b = 0; b < ham_ep; b++)
        {
            
            bit2 = str_re[b];
            index2 = stoi(bit2, 0, 2);
            //cout<<"mpeneis se kanourgio buckets: "<<index2<<endl;
            //cin>>elenxws;
            my_probes--;

            if(my_probes > 0)
            {

                for (int z = 0; z < hypercube_table[index2].size(); z++)
                {
                    my_m--;
                    Point temp_p = hypercube_table[index2][z];
                    double d = euc_dist(temp_p.getCoord(), query.getCoord());

                    if( R > sqrt(d))
                    {
                        list_N.push_back({temp_p,d});
                    }
                    
                    

                    if( my_m == 0)
                    {
                        //cout<<"Teleiwsan ta my_m"<<endl;
                        //cin>>elenxws;
                        //ekptupwseis
                        sort(list_N.begin(), list_N.end(), Compare);
                        out_file<<"--------------------------------"<<endl;
                        out_file<<"Query: "<<query.getID()<<endl;
                        out_file<<"Nearest point within range R"<<R<<": "<<endl;
                        int count=0;
                        
                        for (int t = 0 ; t < list_N.size() ; t++)
                        {          
                            N_near temp_near = list_N[t];
                            out_file<<"Nearest neighbor -"<<t+1
                                    <<" id: "<<temp_near.p.getID()<<endl
                                    <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                            count++;
                            if( count == m )
                            {
                                return;
                            }
                        }

                        return ;
                    }

                    //my_m--;

                }
            }

            //my_probes--;

            if(my_probes <= 0)
            {
                //cout<<"Teleiwsan ta my_probes"<<endl;
                //cin>>elenxws;
                //ekptupwseis
                sort(list_N.begin(), list_N.end(), Compare);
                out_file<<"--------------------------------"<<endl;
                out_file<<"Query: "<<query.getID()<<endl;
                out_file<<"Nearest point within range R"<<R<<": "<<endl;
                int count=0;
                
                for (int t = 0 ; t < list_N.size() ; t++)
                {          
                    N_near temp_near = list_N[t];
                    out_file<<"Nearest neighbor -"<<t+1
                            <<" id: "<<temp_near.p.getID()<<endl
                            <<"Distance: "<<sqrt(temp_near.dis)<<endl;
                    count++;
                    if( count == m )
                    {
                        return;
                    }
                }
                return ;
            }
       }
    }
}

void hypercube::hyper_search_R_All(vector<Point> query_points, int R, int m, int probes, ofstream& out_file){

    for (int i = 0; i < query_points.size(); i++){
        auto start = chrono::high_resolution_clock::now();

        hyper_search_R(query_points[i],R,m,probes,out_file);

        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = stop - start;
        out_file<<"\n Total time taken: "<<elapsed.count()<<endl<<endl;
    }
}

Apotelesma_range hypercube::hyper_search_R_clu(Point query, int R, int m, int probes)
{
    int elenxws;
    
    vector<N_near> list_N;
    unsigned long int index = 0;
    string bit = "";
    int my_m = m;
    int my_probes = probes;
    Apotelesma_range ap;

    //To kanw gia to buckets tou query   
    for(int j=0; j<this->k; j++)
    {
        string temp;

        int bit_f = f_number[j].f_function(query); //returns 0 or 1

        temp = to_string(bit_f);

        bit = bit + temp;
    }

    index = stoi(bit, 0, 2);

    //cout<<"Prwto bucket"<<endl;
    //cin>>elenxws;
    for (int z = 0; z < hypercube_table[index].size(); z++)
    {
        Point temp_p = hypercube_table[index][z];
        if(temp_p.getCluster_ID() == query.getCluster_ID())
        {
            continue;
        }

        double d = euc_dist(temp_p.getCoord(), query.getCoord());
                    
        if( R > sqrt(d))
        {
            //list_N.push_back({temp_p,d});
            if(temp_p.getCluster_ID() == 0)
            {
                ap.kanonika_apot.push_back(temp_p.getID());
            }
            else if( temp_p.getCluster_ID() != query.getCluster_ID())
            {
                ap.bloblhmatika_apot.push_back(temp_p.getID());
                ap.bloblh_kentro.push_back(temp_p.getCluster_ID());
            }
        }


        my_m--;

        

        if( my_m == 0)
        {   
            //cout<<"Brefhkan ola ta stoixeia sto prwto bucket"<<endl;
            //cin>>elenxws;

            //ekptupwseis
            sort(list_N.begin(), list_N.end(), Compare);
            //cout<<"--------------------------------"<<endl;
            //cout<<"Gia to query"<<query.getID()<<endl;
            //cout<<"Ta pio kontina shmeia entos ths aktinas R :"<<R<<"einai :"<<endl;
            int count=0;
            
            for (int t = 0 ; t < list_N.size(); t++)
            {          
                //N_near temp_near = list_N[t];
                //cout<<"O pio kontinos: "<<t+1<<" "<<"exei id:"<<temp_near.p.getID()<<"kai apostash:"<<sqrt(temp_near.dis)<<endl;
                count++;
                
                if( count == m )
                {
                    return ap;
                }
            }

            return ap;
        }

        //my_m--;
        
    }
    //cout<<"Telos prwtou bucket"<<endl;
    //cout<<"Brefhkan mexri twra tosa stoixeia: "<<list_N.size()<<endl;
    //cin>>elenxws;

    my_probes--;

    if( my_probes == 0 )
    {
        //cout<<"2 Brefhkan ola ta stoixeia sto prwto bucket"<<endl;
        //cin>>elenxws;
        //ekptupwseis
        //sort(list_N.begin(), list_N.end(), Compare);
        //cout<<"--------------------------------"<<endl;
        //cout<<"Gia to query"<<query.getID()<<endl;
        //cout<<"Ta pio kontina shmeia entos ths aktinas R :"<<R<<"einai :"<<endl;
        int count=0;
        for (int t = 0 ; t < list_N.size() ; t++)
        {          
            //N_near temp_near = list_N[t];
            //cout<<"O pio kontinos: "<<t+1<<" "<<"exei id:"<<temp_near.p.getID()<<"kai apostash:"<<sqrt(temp_near.dis)<<endl;
            count++;
            if( count == m )
            {
                return ap;
            }
        }

        return ap;
    }

    //Jekinhma diadikasias jafumatos se alla bucket bash hamming distance
    std::vector<char> str;
    for (int i = 0; i < bit.length(); i++)
        {
        	str.push_back(bit[i]);
        	//cout << str[i];
    	}

    bool new_ham = true;
    int len = str.size();
    int maxDistance = this->k;     
            
    while( my_probes > 0 )
    {   
        //Ypologismos twn allwn hamming distance
        
        vector<string> str_re;  
        int index2;
        int index_str;
        string bit2="";
        int ham_ep;
        if( new_ham == true)
        {
            new_ham = false;
            //for (size_t ham = 0; i < this->k; ham++)
            //{
                //cout<<"Ypologismos tou hamming distance"<<"Me maxDistance: "<<maxDistance<<endl;
                //cin>>elenxws;
                str_re.clear();
                for (size_t i = 1 ; i <= maxDistance ; ++i) 
                {
                    //printf("Computing for distance %lu\n", i);
                    hamming_dis_near(str, len-1, i,str_re);
                    //printf("----------------\n");
                }
            //}

            //index_str = rand() % str_re.size();
            
            //bit2 = str_re[index_str];
            //index2 = stoi(bit, 0, 2);
            ham_ep = str_re.size();

        }

        for (int b = 0; b < ham_ep; b++)
        {
            
            bit2 = str_re[b];
            index2 = stoi(bit2, 0, 2);
            //cout<<"mpeneis se kanourgio buckets: "<<index2<<endl;
            //cin>>elenxws;
            my_probes--;

            if(my_probes > 0)
            {

                for (int z = 0; z < hypercube_table[index2].size(); z++)
                {
                    my_m--;
                    Point temp_p = hypercube_table[index2][z];
                    if(temp_p.getCluster_ID() == query.getCluster_ID())
                    {
                        continue;
                    }
                    double d = euc_dist(temp_p.getCoord(), query.getCoord());

                    if( R > sqrt(d))
                    {
                        //list_N.push_back({temp_p,d});
                        if(temp_p.getCluster_ID() == 0)
                        {
                            ap.kanonika_apot.push_back(temp_p.getID());
                        }
                        else if( temp_p.getCluster_ID() != query.getCluster_ID())
                        {
                            ap.bloblhmatika_apot.push_back(temp_p.getID());
                            ap.bloblh_kentro.push_back(temp_p.getCluster_ID());
                        }
                    }
                    
                    

                    if( my_m == 0)
                    {
                        //cout<<"Teleiwsan ta my_m"<<endl;
                        //cin>>elenxws;
                        //ekptupwseis
                        //sort(list_N.begin(), list_N.end(), Compare);
                        //cout<<"--------------------------------"<<endl;
                        //cout<<"Gia to query"<<query.getID()<<endl;
                        //cout<<"Ta pio kontina shmeia entos ths aktinas R :"<<R<<"einai :"<<endl;
                        int count=0;
                        
                        for (int t = 0 ; t < list_N.size() ; t++)
                        {          
                            //N_near temp_near = list_N[t];
                            //cout<<"O pio kontinos: "<<t+1<<" "<<"exei id:"<<temp_near.p.getID()<<"kai apostash:"<<sqrt(temp_near.dis)<<endl;
                            count++;
                            if( count == m )
                            {
                                return ap;
                            }
                        }

                        return ap;
                    }

                    //my_m--;

                }
            }

            //my_probes--;

            if(my_probes <= 0)
            {
                //cout<<"Teleiwsan ta my_probes"<<endl;
                //cin>>elenxws;
                //ekptupwseis
                //sort(list_N.begin(), list_N.end(), Compare);
                //cout<<"--------------------------------"<<endl;
                //cout<<"Gia to query"<<query.getID()<<endl;
                //cout<<"Ta pio kontina shmeia entos ths aktinas R :"<<R<<"einai :"<<endl;
                int count=0;
                
                for (int t = 0 ; t < list_N.size() ; t++)
                {          
                    //N_near temp_near = list_N[t];
                    //cout<<"O pio kontinos: "<<t+1<<" "<<"exei id:"<<temp_near.p.getID()<<"kai apostash:"<<sqrt(temp_near.dis)<<endl;
                    count++;
                    if( count == m )
                    {
                        return ap;
                    }
                }
                return ap;
            }

            //cout<<"Telos bucket"<<endl;
            //cout<<"Brefhkan mexri twra tosa stoixeia: "<<list_N.size()<<endl;
            //cin>>elenxws;

       }
    }
   return ap;
}

void hamming_dis_near(vector<char>& str, const int i, const int changesLeft,vector<string>& str_re) {
        
    if (changesLeft == 0) {      
        string s;
        for (char c: str){
            s.push_back(c);
        }
            str_re.push_back(s);
            return;
    }
    if (i < 0) return;
        // flip current bit
    str[i] ^= 1;
    hamming_dis_near(str, i-1, changesLeft-1,str_re);
        // or don't flip it (flip it again to undo)
    str[i] ^= 1;
    hamming_dis_near(str, i-1, changesLeft,str_re);
}