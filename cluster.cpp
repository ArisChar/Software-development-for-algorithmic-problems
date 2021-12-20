#include "cluster.h"

Cluster::Cluster(int clusterId, Point centroid)
{
    this->clusterId = clusterId;
    this->Cluster_centroid = centroid.getCoord();
    this->addPoint(centroid);
}

Cluster::Cluster(int clusterId, Point centroid ,int metrikh)
{
    this->clusterId = clusterId;
    
    this->curve_centroid = centroid.getCurve();
    this->addPoint(centroid);
}

void Cluster::addPoint(Point p)
{
    p.setCluster(this->clusterId);
    points.push_back(p);
}

bool Cluster::removePoint(int pointId)
{
    int size = points.size();

    for (int i = 0; i < size; i++)
    {
        if (points[i].getID() == pointId)
        {
            points.erase(points.begin() + i);
            return true;
        }
    }
    return false;
}

void Cluster::removeAllPoints() { points.clear(); }

int Cluster::get_clusterId() { return clusterId; }

Point Cluster::getPoint(int pos) { return points[pos]; }

int Cluster::getSize() { return points.size(); }

int Cluster::getSize_Curve() { return curve_centroid.size(); }

double Cluster::getCentroidByPos(int pos) { return Cluster_centroid[pos]; }

vector<double> Cluster::getCentroid() { return Cluster_centroid; }

void Cluster::setCentroidByPos(int pos, double val) { this->Cluster_centroid[pos] = val; }

vector<vector<double>> Cluster::getCurve_Centroid() { return curve_centroid; }

vector<vector<double>> Cluster::getCurve_Curve_index(int index) { return points[index].getCurve();}

void Cluster::setCurve_Centroid(vector<vector<double>> &a)
{
    this->curve_centroid = a;
}

double Cluster::getCurvedByPos_x(int pos) {return curve_centroid[pos][0];}
double Cluster::getCurvedByPos_y(int pos) {return curve_centroid[pos][1];}

//-------------------KMeans------------------
KMeans::KMeans(int K, int iterations, string output_dir)
    {
        this->K = K;
        this->iters = iterations;
        this->output_dir = output_dir;
    }

void KMeans::run_Lloyd(vector<Point> &all_points, int c)
    {      
        int j;
	    int cluster_num;
	    long double sum;
        
        /*Start of k-means++ */
        vector<double> distances;

        auto start = chrono::high_resolution_clock::now();

        //Epilogh prwtou tuxaiou kentrou
        int index = rand() % all_points.size();
        all_points[index].setCluster(1);
        Cluster cluster(1, all_points[index]);
        clusters.push_back(cluster);
        
        //Jekinhma ths kmeans++ arxikopoihseis
	    for (cluster_num = 1; cluster_num < K; cluster_num++)
        {
  
		    sum = 0.0;
		    for ( j = 0; j < all_points.size(); j++ )
            {
                distances.push_back( NearestClusterDistance(all_points[j]));
			    sum += distances[j];
		    }
 
		    sum = sum * rand() / (RAND_MAX - 1);

		    for (j = 0; j < all_points.size(); j++ ) {
			    sum -= distances[j];
			    if ( sum <= 0)
			    {           
                    all_points[j].setCluster(cluster_num + 1);
                    Cluster cluster(cluster_num + 1, all_points[j]);
                    clusters.push_back(cluster);
				    break;
			    }
		    }
	    }
        //Telos ths arxikopoihshs
       
        int epanalhjh = 1;
        while (true)
        {
            bool mh_termatismos = true;

            //Ebresh se ola ta points ta kontina kentra cluster
            
            for (int i = 0; i < all_points.size(); i++)
            {
                int currentClusterId = all_points[i].getCluster_ID();
                int nearestClusterId = getNearestClusterId(all_points[i]);

                if (currentClusterId != nearestClusterId)
                {
                    all_points[i].setCluster(nearestClusterId);
                    mh_termatismos = false;
                }
            }

            //Adeiasma twn cluster apo wste nas mporei na epanalhfxh olh h diadikasia
            clearClusters();

            //Topofethsh(prwth olikh) olwn twn points sta kontina clusters
            for (int i = 0; i < all_points.size(); i++)
            {
                // cluster index is ID-1
                clusters[all_points[i].getCluster_ID() - 1].addPoint(all_points[i]);
            }

            // Recalculating the center of each cluster
            for (int i = 0; i < K; i++)
            {
                
                int ClusterSize = clusters[i].getSize();

                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    long double sum = 0.0;
                    if (ClusterSize > 0)
                    {
                        
                        for (int p = 0; p < ClusterSize; p++)
                        {
                            sum += clusters[i].getPoint(p).getVal(j);
                        }
                        clusters[i].setCentroidByPos(j, sum / ClusterSize);
                    }   
                }
            }

            if (mh_termatismos || epanalhjh >= iters)
            {
                //cout <<"Clustering teleiwse se  : "<<epanalhjh<<" kuklous"<<endl<< endl;            
                break;
            }
            epanalhjh++;
        }
        auto stop = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = stop - start;

        //Silhouette
              
        long double sum_s=0;
        long double average=0;
        long double temp_dist;
        vector<long double> dist_s;
        vector<long double> si;
        vector<vector<long double>> si_k;

        //c.resize(a.size(), vector<double>((b.size())));
        si_k.resize(K);
        for (int q = 0; q < all_points.size(); q++)
        {
            
                        
            
            for (int n = 0; n < K; n++)
            {
                dist_s.push_back( euc_dist(clusters[n].getCentroid(),all_points[q].getCoord()));

            }       
            sort(dist_s.begin(), dist_s.end(), sort_using);
            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];
            //sum_s = sum_s + temp_dist;
            si_k[all_points[q].getCluster_ID()-1].push_back(temp_dist);

            //si.push_back(temp_dist);
            dist_s.clear();
            
        }
        long double av_si=0;
        for (int i1 = 0; i1 < K; i1++)
        {
            for (int i2 = 0; i2 < si_k[i1].size(); i2++)
            {
                av_si = av_si + si_k[i1][i2];
            }
            av_si = av_si/si_k[i1].size();
            si.push_back(av_si);
            average = average + temp_dist;
            av_si = 0;
        }
        average = average / K;

        //Eggrafh se arxeio
        ofstream outfile;
        outfile.open(output_dir);

        outfile<<"Algorithm: Lloyds"<<endl;

        if (outfile.is_open())
        {
            for (int i = 0; i < K; i++)
            {
                outfile << "CLUSTER-"<<clusters[i].get_clusterId()<<"{size: "<<clusters[i].getSize()<<  ", centroid : ";
                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    outfile<<" || "<< clusters[i].getCentroidByPos(j); // Output to file
                }
                outfile<<"}\n"<< endl;
            }

            outfile<<"clustering_time: "<<elapsed.count()<<endl<<endl;
            outfile<<"Silhouette: [";
            for (int i = 0; i < si.size(); i++)
            {
                outfile<<si[i]<<", ";
            }
            outfile<<" stotal: ";
            outfile<<average<<"]\n";

            if(c == 1){
                for (int i = 0; i < K; i++){
                    outfile << "\nCLUSTER-"<<clusters[i].get_clusterId()<<"{centroid";

                    for (int j = 0; j < clusters[i].getSize(); j++)
                    {
                        outfile<<", "<< (clusters[i].getPoint(j)).getID();
                    }

                    outfile << "}\n";
                }
            }

            outfile.close();
        }
        else
        {
            cout << "Error: Unable to write";
        }
    }

void KMeans::run_Lloyd_DFD(vector<Point> &all_points, int c)
    {      
        int j;
	    int cluster_num;
	    long double sum;
        
        /*Start of k-means++ */
        vector<double> distances;
        vector<int> SK;
        std::vector<int>::iterator it;

        auto start = chrono::high_resolution_clock::now();

        //Epilogh prwtou tuxaiou kentrou
        int index = rand() % all_points.size();
        SK.push_back(all_points[index].getID());
        all_points[index].setCluster(1);
        //Cluster cluster(1, all_points[index]);
        Cluster cluster(1,all_points[index],2);
        clusters.push_back(cluster);
        bool arx_epan =true;

        //cout<<"Prin thn arxikopoihsei"<<endl;
        //Jekinhma ths kmeans++ arxikopoihseis
	    for (cluster_num = 1; cluster_num < K; cluster_num++)
        {
            arx_epan =true;
            //cout<<"Arxikopoihsei ths korudhs: "<<cluster_num<<endl;
		    sum = 0.0;
		    for ( j = 0; j < all_points.size(); j++ )
            {
                //cout<<"j: "<<j<<endl;
                distances.push_back( NearestClusterDistance_DFD(all_points[j]));
			    sum += distances[j];
                
		    }
            
 
		    sum = sum * rand() / (RAND_MAX - 1);
            sum = sum / cluster_num;

		    for (j = 0; j < all_points.size(); j++ ) {
			    sum -= distances[j];
			    if ( sum <= 0)
			    {   
                    while (arx_epan)
                    {
                        // cout<<"Arxh Deutero eswteriko for "<<endl;
                        it = std::find (SK.begin(), SK.end(), j);
                        if (it != SK.end())
                        {
                            //cout<<"Yparxei"<<endl;
                             j=j-10;          
                        }
                        else
                        {
                            SK.push_back(j);
                            //cout<<"Den Yparxei"<<endl;
                            all_points[j].setCluster(cluster_num + 1);
                            Cluster cluster(cluster_num + 1, all_points[j],2);
                            clusters.push_back(cluster);
                            arx_epan =false;
                            //cout<<"j:"<<j<<endl;
				            break;
                        }
                  
                    }
                 
                    if (arx_epan == false)
                    {
                        break;
                    }
			    }
		    }
            distances.clear();
	    }
        //Telos ths arxikopoihshs
        
        int epanalhjh = 1;
        vector<vector<vector<double>>> all_curve;
        while (true)
        {
            //cout<<"Epanalhjh noumero: "<<epanalhjh<<endl;

            bool mh_termatismos = true;

            //Ebresh se ola ta points ta kontina kentra cluster
            //cout<<"Arxh Ebresh se ola ta points ta kontina kentra cluster"<<endl;
            for (int i = 0; i < all_points.size(); i++)
            {
                
                int currentClusterId = all_points[i].getCluster_ID();
                int nearestClusterId = getNearestClusterId_DFD(all_points[i]);

                if (currentClusterId != nearestClusterId)
                {
                    all_points[i].setCluster(nearestClusterId);
                    mh_termatismos = false;
                }
                
            }
                
            //cout<<"Telos Ebresh se ola ta points ta kontina kentra cluster"<<endl;
            //Adeiasma twn cluster apo wste nas mporei na epanalhfxh olh h diadikasia
            clearClusters();

            
            //Topofethsh(prwth olikh) olwn twn points sta kontina clusters
            for (int i = 0; i < all_points.size(); i++)
            {
                // cluster index is ID-1
                clusters[all_points[i].getCluster_ID() - 1].addPoint(all_points[i]);
            }
            

            // Recalculating the center of each cluster
            //cout<<"Arxh Recalculating the center of each cluster"<<endl;

            vector<vector<double>> temp;
            for (int i = 0; i < K; i++)
            {     
                
                for (int j = 0; j < all_points.size() ; j++)
                {   
                    if( all_points[j].getCluster_ID() == i+1 )
                    {
                        all_curve.push_back(all_points[j].getCurve());
                    }
                   
                }
                if(all_curve.size()>1)
                {
                    //cout<<"all_curve.size"<<all_curve.size()<<endl;
                    temp = mean_curve_many(all_curve);
                    clusters[i].setCurve_Centroid(temp);
                    all_curve.clear();
                }
                if(all_curve.size()==1)
                {
                    all_curve.clear();
                }
                
                
            }
            
            

            if (mh_termatismos || epanalhjh >= iters)
            {
                //cout <<"Clustering teleiwse se  : "<<epanalhjh<<" kuklous"<<endl<< endl;            
                break;
            }
            epanalhjh++;
            
        }
        
        auto stop = chrono::high_resolution_clock::now();

        chrono::duration<double> elapsed = stop - start;

        //Silhouette

        long double sum_s=0;
        long double average=0;
        long double temp_dist;
        vector<long double> dist_s;
        vector<long double> si;
        vector<vector<long double>> si_k;

        //c.resize(a.size(), vector<double>((b.size())));
        si_k.resize(K);
        for (int q = 0; q < all_points.size(); q++)
        {
            
                        
            
            for (int n = 0; n < K; n++)
            {
                    dist_s.push_back( DiscreteFrechet(clusters[n].getCurve_Centroid(),all_points[q].getCurve()));

            }       
            sort(dist_s.begin(), dist_s.end(), sort_using);
            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];
            //sum_s = sum_s + temp_dist;
            si_k[all_points[q].getCluster_ID()-1].push_back(temp_dist);

            //si.push_back(temp_dist);
            dist_s.clear();
            
        }
        long double av_si=0;
        for (int i1 = 0; i1 < K; i1++)
        {
            for (int i2 = 0; i2 < si_k[i1].size(); i2++)
            {
                av_si = av_si + si_k[i1][i2];
            }
            av_si = av_si/si_k[i1].size();
            si.push_back(av_si);
            average = average + temp_dist;
            av_si = 0;
        }
        average = average / K;
             
        //Eggrafh se arxeio
       
        ofstream outfile;
        outfile.open(output_dir);

        outfile<<"Algorithm: Lloyds"<<endl;

        if (outfile.is_open())
        {
            for (int i = 0; i < K; i++)
            {
                outfile << "CLUSTER-"<<clusters[i].get_clusterId()<<"{size: "<<clusters[i].getSize()<<  ", centroid : ";
                for (int j = 0; j < clusters[i].getSize_Curve(); j++)
                {
                     
                    //outfile<<" || "<< clusters[i].getCentroidByPos(j); // Output to file
                    outfile<<" || "<< clusters[i].getCurvedByPos_x(j)<<" , "<<clusters[i].getCurvedByPos_y(j);
                    
                }
                outfile<<"}\n"<< endl;
            }

            outfile<<"clustering_time: "<<elapsed.count()<<endl<<endl;
            outfile<<"Silhouette: [";
            for (int i = 0; i < si.size(); i++)
            {
                outfile<<si[i]<<", ";
            }
            outfile<<" stotal: ";
            outfile<<average<<"]\n";
            
            if(c == 1){
                for (int i = 0; i < K; i++){
                    outfile << "\nCLUSTER-"<<clusters[i].get_clusterId()<<"{centroid";
                    //cout<<"clusters["<<i<<"]"<<"getSize_Curve()"<<clusters[i].getSize_Curve()<<endl;
                    for (int j = 0; j < clusters[i].getSize(); j++)
                    {
                        //cout<<"Edw"<<endl;
                        outfile<<", "<< (clusters[i].getPoint(j)).getID();
                    }

                    outfile << "}\n";
                }
            }

            outfile.close();
        }
        else
        {
            cout << "Error: Unable to write";
        }
    }    

void KMeans::clearClusters()
    {
        for (int i = 0; i < K; i++)
        {
            clusters[i].removeAllPoints();
        }
    }

int KMeans::getNearestClusterId(Point point)
    {
        long double  min_dist;
        int NearestClusterId;

        
        min_dist = euc_dist(clusters[0].getCentroid(),point.getCoord());

        NearestClusterId = clusters[0].get_clusterId();

        if(this->clusters.size() == 1)
        {
            return NearestClusterId;
        }

        long double dist_of_other;

        for (int i = 1; i < this->clusters.size(); i++)
        {
            dist_of_other = euc_dist(clusters[i].getCentroid(),point.getCoord());

            if (dist_of_other < min_dist)
            {
                min_dist = dist_of_other;
                NearestClusterId = clusters[i].get_clusterId();
            }
        }

        return NearestClusterId;
    }

    int KMeans::getNearestClusterId_DFD(Point point)
    {
        long double  min_dist;
        int NearestClusterId;

        
        //min_dist = euc_dist(clusters[0].getCentroid(),point.getCoord());
        min_dist = DiscreteFrechet(clusters[0].getCurve_Centroid(),point.getCurve());

        NearestClusterId = clusters[0].get_clusterId();

        if(this->clusters.size() == 1)
        {
            return NearestClusterId;
        }

        long double dist_of_other;

        for (int i = 1; i < this->clusters.size(); i++)
        {
            //dist_of_other = euc_dist(clusters[i].getCentroid(),point.getCoord());
            dist_of_other = DiscreteFrechet(clusters[i].getCurve_Centroid(),point.getCurve());

            if (dist_of_other < min_dist)
            {
                min_dist = dist_of_other;
                NearestClusterId = clusters[i].get_clusterId();
            }
        }

        return NearestClusterId;
    }

long double KMeans::NearestClusterDistance(Point point)
    {
        long double min_dist;
        int NearestClusterId;

        min_dist = euc_dist(clusters[0].getCentroid(),point.getCoord());

        if (this->clusters.size() == 1)
        {
            return min_dist;
        }
        
        long double dist_of_other;
        for (int i = 1; i < this->clusters.size() ; i++)
        {
  
            dist_of_other = euc_dist(clusters[i].getCentroid(),point.getCoord());
            if (dist_of_other < min_dist)
            {
                min_dist = dist_of_other;
            }
        }

        return min_dist;
    }

    long double KMeans::NearestClusterDistance_DFD(Point point)
    {
        long double min_dist;
        int NearestClusterId;

        //min_dist = euc_dist(clusters[0].getCentroid(),point.getCoord());
        min_dist = DiscreteFrechet(clusters[0].getCurve_Centroid(),point.getCurve());

        if (this->clusters.size() == 1)
        {
            return min_dist;
        }
        
        long double dist_of_other;
        
        for (int i = 1; i < this->clusters.size() ; i++)
        {
            
            //dist_of_other = euc_dist(clusters[i].getCentroid(),point.getCoord());
            dist_of_other = DiscreteFrechet(clusters[i].getCurve_Centroid(),point.getCurve());
            
            if (dist_of_other < min_dist)
            {
                min_dist = dist_of_other;
            }
        }

        return min_dist;
    }




void KMeans::run_LSH(vector<Point> &all_points,int k_hash,int L_hash, int c)
    {
        int j;
	    int cluster_num;
	    long double sum;
  

        /*Start of k-means++ */
        vector<double> distances;

        
        //Epilogh prwtou tuxaiou kentrou
        int index = rand() % all_points.size();
        all_points[index].setCluster(1);
        Cluster cluster(1, all_points[index]);
        clusters.push_back(cluster);
        
        //Jekinhma ths kmeans++ arxikopoihseis
	    for (cluster_num = 1; cluster_num < K; cluster_num++)
        {    
		    sum = 0.0;
		    for ( j = 0; j < all_points.size(); j++ )
            {
                distances.push_back( NearestClusterDistance(all_points[j]));
			    sum += distances[j];
		    }
 
		    sum = sum * rand() / (RAND_MAX - 1);
 		
		    for (j = 0; j < all_points.size(); j++ )
            {
			    sum -= distances[j];
			    if ( sum <= 0)
			    {
                    all_points[j].setCluster(cluster_num + 1);
                    Cluster cluster(cluster_num + 1, all_points[j]);
                    clusters.push_back(cluster);
				    break;
			    }
		    }
	    }
        //Telos ths arxikopoihshs

        //cout<<"Jekinaei to clustering me bohfeia tou algorimou LSH"<<endl;
        auto start = chrono::high_resolution_clock::now();
        //Jekiname tou clustering me bohfeia tou algorimou LSH
        lsh l(k_hash,L_hash,all_points[0].getDimensions(),all_points.size());
        l.lsh_tables_create(all_points);

        int epanalhjh = 1;
        Apotelesma_range ap;
        int R = 100;    //Arxikh aktina anazuthseis
        int count_points=0;
        //Reverse assignment
        while(true)
        {
            //Efarmogh tou range search gia kafe kentro
            for (size_t z = 0; z < K; z++)
            {
                Point query(clusters[z].getCentroid(),clusters[z].get_clusterId());
                ap = l.query_range_search_clu(query,R);
                count_points =count_points + ap.kanonika_apot.size();
                count_points =count_points + ap.bloblhmatika_apot.size();

                for (int u = 0; u < ap.kanonika_apot.size(); u++)
                {
                    if(all_points[ap.kanonika_apot[u]-1].getCluster_ID() == 0)
                    {
                        all_points[ap.kanonika_apot[u]-1].setCluster(clusters[z].get_clusterId());
                        clusters[z].addPoint(all_points[ap.kanonika_apot[u]-1]);
                    }
                }

                if( ap.bloblhmatika_apot.size() > 0)
                {
                    long double dist_poin_ken1,dist_poin_ken2;

                    for (int b = 0; b < ap.bloblhmatika_apot.size(); b++)
                    {
                        int shm_k = ap.bloblh_kentro[b] - 1;
                        //neo kentro
                        dist_poin_ken1 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[z].getCentroid());
                        //isxuwn kentro
                        dist_poin_ken2 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[shm_k].getCentroid());
                        if(dist_poin_ken1 > dist_poin_ken2)
                        {
                            //Uparxei gia logikh bohfeia
                        }
                        else if (dist_poin_ken1 < dist_poin_ken2)
                        {
                            all_points[ap.bloblhmatika_apot[b]-1].setCluster(clusters[z].get_clusterId());
                            clusters[z].addPoint(all_points[ap.bloblhmatika_apot[b]-1]);
                        }
                        
                    }
                                                       
                }
           
            }

        
            //Ypologismos tou neou kentrou gia kafe cluster
            for (int i = 0; i < K; i++)
            {              
                int ClusterSize = clusters[i].getSize();

                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    long double sum = 0.0;
                    if (ClusterSize > 0)
                    {                   
                        for (int p = 0; p < ClusterSize; p++)
                        {
                            sum += clusters[i].getPoint(p).getVal(j);
                        }
                        clusters[i].setCentroidByPos(j, sum / ClusterSize);
                    }   
                }
            }

            //Sunfhkes gia termatismo tou range serach sth LSH
            if(count_points < 10 && epanalhjh > 25)
            {       
                //cout<<"Clustering teleiwse bash ths prwths sunfhkhs"<<count_points<<endl<<endl;
                break;
            }
  
            R = R*2;
            epanalhjh++;
            count_points=0;

        }

        //Topofethsh twn shmeiwn xwriss kentro
        for (int i = 0; i < all_points.size(); i++)
            {
                int currentClusterId = all_points[i].getCluster_ID();
                if(currentClusterId == 0)
                {
                    int nearestClusterId = getNearestClusterId(all_points[i]);
                    all_points[i].setCluster(nearestClusterId);
                    clusters[nearestClusterId-1].addPoint(all_points[i]);
                }
                
            }

        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = stop - start;

        //Silhouette

        long double sum_s=0;
        long double average=0;
        long double temp_dist;
        vector<long double> dist_s;
        vector<long double> si;
        vector<vector<long double>> si_k;

        //c.resize(a.size(), vector<double>((b.size())));
        si_k.resize(K);
        for (int q = 0; q < all_points.size(); q++)
        {
            
                        
            
            for (int n = 0; n < K; n++)
            {
                dist_s.push_back( euc_dist(clusters[n].getCentroid(),all_points[q].getCoord()));

            }       
            sort(dist_s.begin(), dist_s.end(), sort_using);
            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];
            //sum_s = sum_s + temp_dist;
            si_k[all_points[q].getCluster_ID()-1].push_back(temp_dist);

            //si.push_back(temp_dist);
            dist_s.clear();
            
        }
        long double av_si=0;
        for (int i1 = 0; i1 < K; i1++)
        {
            for (int i2 = 0; i2 < si_k[i1].size(); i2++)
            {
                av_si = av_si + si_k[i1][i2];
            }
            av_si = av_si/si_k[i1].size();
            si.push_back(av_si);
            average = average + temp_dist;
            av_si = 0;
        }
        average = average / K;

        //Eggrafh se arxeio
        ofstream outfile;
        outfile.open(output_dir);

        outfile<<"Algorithm: LSH"<<endl;

        if (outfile.is_open())
        {
            for (int i = 0; i < K; i++)
            {
                outfile << "CLUSTER-"<<clusters[i].get_clusterId()<<"{size: "<<clusters[i].getSize()<<  ", centroid : ";
                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    outfile<<" || "<< clusters[i].getCentroidByPos(j); // Output to file
                }
                outfile<<"}\n"<< endl;
            }

            outfile<<"clustering_time: "<<elapsed.count()<<endl<<endl;
            outfile<<"Silhouette: [";
            for (int i = 0; i < si.size(); i++)
            {
                outfile<<si[i]<<", ";
            }
            outfile<<" stotal: ";
            outfile<<average<<"]\n";

            if(c == 1){
                for (int i = 0; i < K; i++){
                    outfile << "\nCLUSTER-"<<clusters[i].get_clusterId()<<"{centroid";

                    for (int j = 0; j < clusters[i].getSize(); j++)
                    {
                        outfile<<", "<< (clusters[i].getPoint(j)).getID();
                    }

                    outfile << "}\n";
                }
            }
        }
        else
        {
            cout << "Error: Unable to write to clusters.txt";
        }
    }

void KMeans::run_LSH_DFD(vector<Point> &all_points,int k_hash,int L_hash, int c)
    {
        int j;
	    int cluster_num;
	    long double sum;
  

        /*Start of k-means++ */
        vector<double> distances;
        vector<int> SK;
        std::vector<int>::iterator it;
        
        //Epilogh prwtou tuxaiou kentrou
        int index = rand() % all_points.size();
        SK.push_back(all_points[index].getID());
        all_points[index].setCluster(1);
        //Cluster cluster(1, all_points[index]);
        Cluster cluster(1,all_points[index],2);
        clusters.push_back(cluster);
        bool arx_epan =true;

        //cout<<"Prin thn arxikopoihsei"<<endl;
        //Jekinhma ths kmeans++ arxikopoihseis
	    for (cluster_num = 1; cluster_num < K; cluster_num++)
        {   
            arx_epan =true;
            //cout<<"Arxikopoihsei ths korudhs: "<<cluster_num<<endl;
		    sum = 0.0;
            //cout<<"Arxh Prwtou for "<<endl;
		    for ( j = 0; j < all_points.size(); j++ )
            {
                //distances.push_back( NearestClusterDistance(all_points[j]));
                distances.push_back( NearestClusterDistance_DFD(all_points[j]));
			    sum += distances[j];
		    }
            //cout<<"Telos Prwtou for "<<endl;

		    sum = sum * rand() / (RAND_MAX - 1);
            sum = sum / cluster_num;
 		
		    for (j = 0; j < all_points.size(); j++ )
            {
			    sum -= distances[j];
			    if ( sum <= 0)
			    {   
                    while (arx_epan)
                    {
                        // cout<<"Arxh Deutero eswteriko for "<<endl;
                        it = std::find (SK.begin(), SK.end(), j);
                        if (it != SK.end())
                        {
                            //cout<<"Yparxei"<<endl;
                             j=j-10;          
                        }
                        else
                        {
                            SK.push_back(j);
                            //cout<<"Den Yparxei"<<endl;
                            all_points[j].setCluster(cluster_num + 1);
                            Cluster cluster(cluster_num + 1, all_points[j],2);
                            clusters.push_back(cluster);
                            arx_epan =false;
                            //cout<<"j:"<<j<<endl;
                            // int tt;
                            // cin>>tt;
				            break;
                        }
                  
                    }

                    //cout<<"Telos Deutero eswteriko for "<<endl;
                    if (arx_epan == false)
                    {
                        break;
                    }
         
			    }
		    }
            distances.clear();
	    }
        //Telos ths arxikopoihshs

        //cout<<"Jekinaei to clustering me bohfeia tou algorimou LSH"<<endl;
        auto start = chrono::high_resolution_clock::now();
        //Jekiname tou clustering me bohfeia tou algorimou LSH

        //cout<<"Arxh proetoimasias lsh"<<endl;
        To2d(all_points);

        lsh l(k_hash,L_hash,all_points[0].getDimensions(),all_points.size());
        l.lsh_tables_create(all_points);

        int epanalhjh = 1;
        Apotelesma_range ap;
        int R = 1;    //Arxikh aktina anazuthseis
        int count_points=0;

        //cout<<"Telos proetoimasias lsh"<<endl;
        //cout<<"Edw"<<endl;
        vector<Point> points_q;
        vector<vector<vector<double>>> all_curve;
        //Reverse assignment
        while(true)
        {   
            //cout<<"Epanalhjh noumero: "<<epanalhjh<<endl;
            //cout<<"Arxh Kuklikh ebriash"<<endl;
            //Efarmogh tou range search gia kafe kentro
            for (size_t z = 0; z < K; z++)
            {
                //cout<<"Dhmiourgia Point"<<endl;
                //Point query(clusters[z].getCentroid(),clusters[z].get_clusterId());
                Point query(clusters[z].getCurve_Centroid(),clusters[z].get_clusterId(),2);
                // int tt;
                // cin>>tt;
                //cout<<"points_q.push_back(query);"<<endl;
                points_q.push_back(query);
                //cout<<"To2d(points_q);"<<endl;
                To2d(points_q);
                //cout<<"ap = l.query_range_search_clu_DFD(points_q[0],R);"<<endl;
                ap = l.query_range_search_clu_DFD(points_q[0],R);
                count_points =count_points + ap.kanonika_apot.size();
                count_points =count_points + ap.bloblhmatika_apot.size();
                //cout<<"Arxh for (int u = 0; u < ap.kanonika_apot.size(); u++)"<<endl;
                for (int u = 0; u < ap.kanonika_apot.size(); u++)
                {
                    if(all_points[ap.kanonika_apot[u]-1].getCluster_ID() == 0)
                    {
                        //cout<<"Arxh Eswterika tou if(all_points[ap.kanonika_apot[u]-1].getCluster_ID() == 0)"<<endl;
                        
                        all_points[ap.kanonika_apot[u]-1].setCluster(clusters[z].get_clusterId());
                        clusters[z].addPoint(all_points[ap.kanonika_apot[u]-1]);

                        //cout<<"Telos Eswterika tou if(all_points[ap.kanonika_apot[u]-1].getCluster_ID() == 0)"<<endl;
                    }
                }
                //cout<<"Telos for (int u = 0; u < ap.kanonika_apot.size(); u++)"<<endl;
                //cout<<"Arxh if( ap.bloblhmatika_apot.size() > 0)"<<endl;
                if( ap.bloblhmatika_apot.size() > 0)
                {
                    long double dist_poin_ken1,dist_poin_ken2;

                    //cout<<"Arxh for (int b = 0; b < ap.bloblhmatika_apot.size(); b++)"<<endl;
                    for (int b = 0; b < ap.bloblhmatika_apot.size(); b++)
                    {
                        int shm_k = ap.bloblh_kentro[b] - 1;
                        //neo kentro
                        //dist_poin_ken1 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[z].getCentroid());
                        dist_poin_ken1 = DiscreteFrechet(all_points[ap.bloblhmatika_apot[b]-1].getCurve(),clusters[z].getCurve_Centroid());
                        //isxuwn kentro
                        //dist_poin_ken2 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[shm_k].getCentroid());
                        dist_poin_ken2 = DiscreteFrechet(all_points[ap.bloblhmatika_apot[b]-1].getCurve(),clusters[shm_k].getCurve_Centroid());
                        if(dist_poin_ken1 > dist_poin_ken2)
                        {
                            //Uparxei gia logikh bohfeia
                        }
                        else if (dist_poin_ken1 < dist_poin_ken2)
                        {
                            all_points[ap.bloblhmatika_apot[b]-1].setCluster(clusters[z].get_clusterId());
                            clusters[z].addPoint(all_points[ap.bloblhmatika_apot[b]-1]);
                        }
                        
                    }
                    //cout<<"Telos for (int b = 0; b < ap.bloblhmatika_apot.size(); b++)"<<endl;
                                                       
                }
                //cout<<"Telos if( ap.bloblhmatika_apot.size() > 0)"<<endl;
                points_q.clear();
            }
            //cout<<"Telos Kuklikh ebriash"<<endl;

            //cout<<"Arxh Recalculating the center of each cluster"<<endl;
            //vector<vector<vector<double>>> all_curve;
            vector<vector<double>> temp;
            for (int i = 0; i < K; i++)
            {     
                
                for (int j = 0; j < all_points.size() ; j++)
                {   
                    if( all_points[j].getCluster_ID() == i+1 )
                    {
                        all_curve.push_back(all_points[j].getCurve());
                    }
                   
                }
                //cout<<"Prin mean_curve_many"<<endl;
                if(all_curve.size()>1)
                {
                    //cout<<"all_curve.size"<<all_curve.size()<<endl;
                    temp = mean_curve_many(all_curve);
                    clusters[i].setCurve_Centroid(temp);
                    //cout<<"Meta mean_curve_many"<<endl;
                    //cout<<"Prin all_curve.clear();"<<endl;
                    all_curve.clear();
                    //cout<<"Meta all_curve.clear();"<<endl;
                }
                if(all_curve.size()==1)
                {
                    //cout<<"Prin all_curve.clear();"<<endl;
                    all_curve.clear();
                    //cout<<"Meta all_curve.clear();"<<endl;
                }
                
                
            }
            //cout<<"Telos Recalculating the center of each cluster"<<endl;

            //Sunfhkes gia termatismo tou range serach sth LSH
            //if(count_points < 10 && epanalhjh > 25)
            //if(true)
            if( epanalhjh > 20)
            {       
                //cout<<"Clustering teleiwse bash ths prwths sunfhkhs"<<count_points<<endl<<endl;
                break;
            }
  
            R = R*2;
            epanalhjh++;
            count_points=0;

        }
        //cout<<"Telos while"<<endl;

        //Topofethsh twn shmeiwn xwriss kentro
        for (int i = 0; i < all_points.size(); i++)
            {
                int currentClusterId = all_points[i].getCluster_ID();
                if(currentClusterId == 0)
                {
                    int nearestClusterId = getNearestClusterId(all_points[i]);
                    all_points[i].setCluster(nearestClusterId);
                    clusters[nearestClusterId-1].addPoint(all_points[i]);
                }
                
            }

        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = stop - start;

        //Silhouette

        long double sum_s=0;
        long double average=0;
        long double temp_dist;
        vector<long double> dist_s;
        vector<long double> si;
        vector<vector<long double>> si_k;

        //c.resize(a.size(), vector<double>((b.size())));
        si_k.resize(K);
        for (int q = 0; q < all_points.size(); q++)
        {
            
                        
            
            for (int n = 0; n < K; n++)
            {
                    dist_s.push_back( DiscreteFrechet(clusters[n].getCurve_Centroid(),all_points[q].getCurve()));

            }       
            sort(dist_s.begin(), dist_s.end(), sort_using);
            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];
            //sum_s = sum_s + temp_dist;
            si_k[all_points[q].getCluster_ID()-1].push_back(temp_dist);

            //si.push_back(temp_dist);
            dist_s.clear();
            
        }
        long double av_si=0;
        for (int i1 = 0; i1 < K; i1++)
        {
            for (int i2 = 0; i2 < si_k[i1].size(); i2++)
            {
                av_si = av_si + si_k[i1][i2];
            }
            av_si = av_si/si_k[i1].size();
            si.push_back(av_si);
            average = average + temp_dist;
            av_si = 0;
        }
        average = average / K;

        //Eggrafh se arxeio
        ofstream outfile;
        outfile.open(output_dir);

        outfile<<"Algorithm: LSH"<<endl;

        if (outfile.is_open())
        {
            for (int i = 0; i < K; i++)
            {
                outfile << "CLUSTER-"<<clusters[i].get_clusterId()<<"{size: "<<clusters[i].getSize()<<  ", centroid : ";
                //for (int j = 0; j < all_points[0].getDimensions(); j++)
                for (int j = 0; j < clusters[i].getSize_Curve(); j++)
                {
                    //outfile<<" || "<< clusters[i].getCentroidByPos(j); // Output to file
                    outfile<<" || "<< clusters[i].getCurvedByPos_x(j)<<" , "<<clusters[i].getCurvedByPos_y(j);
                }
                outfile<<"}\n"<< endl;
            }

            outfile<<"clustering_time: "<<elapsed.count()<<endl<<endl;
            outfile<<"Silhouette: [";
            for (int i = 0; i < si.size(); i++)
            {
                outfile<<si[i]<<", ";
            }
            outfile<<" stotal: ";
            outfile<<average<<"]\n";

            if(c == 1){
                for (int i = 0; i < K; i++){
                    outfile << "\nCLUSTER-"<<clusters[i].get_clusterId()<<"{centroid";
                    //cout<<"clusters[ "<<i<<"]"<<"getSize_Curve()"<<clusters[i].getSize_Curve()<<endl;
                    for (int j = 0; j < clusters[i].getSize(); j++)
                    {
                        outfile<<", "<< (clusters[i].getPoint(j)).getID();
                    }

                    outfile << "}\n";
                }
            }
        }
        else
        {
            cout << "Error: Unable to write to clusters.txt";
        }
    }

void KMeans::run_CUBE(vector<Point> &all_points,int d,int m_buckets ,int probes, int c)
    {
        int j;
	    int cluster_num;
	    long double sum;

        /*Start of k-means++ */

        vector<double> distances;
        auto start = chrono::high_resolution_clock::now();

        //Epilogh prwtou tuxaiou kentrou
        int index = rand() % all_points.size();
        all_points[index].setCluster(1);
        Cluster cluster(1, all_points[index]);
        clusters.push_back(cluster);
        
        //Jekinhma ths kmeans++ arxikopoihseis
	    for (cluster_num = 1; cluster_num < K; cluster_num++)
        {
        
		    sum = 0.0;
		    for ( j = 0; j < all_points.size(); j++ )
            {
                distances.push_back( NearestClusterDistance(all_points[j]));
			    sum += distances[j];
		    }
 
		    sum = sum * rand() / (RAND_MAX - 1);
 

		    for (j = 0; j < all_points.size(); j++ ) 
            {
			    sum -= distances[j];
			    if ( sum <= 0)
			    {
                    all_points[j].setCluster(cluster_num + 1);
                    Cluster cluster(cluster_num + 1, all_points[j]);
                    clusters.push_back(cluster);
				    break;
			    }
		    }
	    }
        //Telos ths arxikopoihshs

        //cout <<"Jekinaei to clustering me bohfeia tou algorimou Hypercube"<< endl;
      
        //Jekinaei to clustering me bohfeia tou algorimou Hypercube
        hypercube hype(d,all_points[0].getDimensions());
        hype.hypercube_input(all_points);

        int epanalhjh = 1;
        Apotelesma_range ap;
        int R = 100;
        int count_points=0;
        //Reverse assignment
        while(true)
        {
            
            for (size_t z = 0; z < K; z++)
            {
                //Efarmogh tou range search gia kafe kentro
                Point query(clusters[z].getCentroid(),clusters[z].get_clusterId());
                ap = hype.hyper_search_R_clu(query,R,m_buckets,probes);
                count_points = count_points + ap.kanonika_apot.size();
                count_points = count_points + ap.bloblhmatika_apot.size();

                for (int u = 0; u < ap.kanonika_apot.size(); u++)
                {
                    if(all_points[ap.kanonika_apot[u]-1].getCluster_ID() == 0)
                    {
                        all_points[ap.kanonika_apot[u]-1].setCluster(clusters[z].get_clusterId());
                        clusters[z].addPoint(all_points[ap.kanonika_apot[u]-1]);
                    }
                }

                if( ap.bloblhmatika_apot.size() > 0)
                {
                    long double dist_poin_ken1,dist_poin_ken2;

                    for (int b = 0; b < ap.bloblhmatika_apot.size(); b++)
                    {
                        int shm_k = ap.bloblh_kentro[b] - 1;
                        //neo kentro
                        dist_poin_ken1 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[z].getCentroid());
                        //isxuwn kentro
                        dist_poin_ken2 = euc_dist(all_points[ap.bloblhmatika_apot[b]-1].getCoord(),clusters[shm_k].getCentroid());
                        if(dist_poin_ken1 > dist_poin_ken2)
                        {
                            //Uparxei gia logikh bohfeia
                        }
                        else if (dist_poin_ken1 < dist_poin_ken2)
                        {
                            all_points[ap.bloblhmatika_apot[b]-1].setCluster(clusters[z].get_clusterId());
                            clusters[z].addPoint(all_points[ap.bloblhmatika_apot[b]-1]);
                        }                     
                    }                                                    
                }
            }

            //Ypologismos tou neou kentrou gia kafe cluster
            for (int i = 0; i < K; i++)
            {
                
                int ClusterSize = clusters[i].getSize();

                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    long double sum = 0.0;
                    if (ClusterSize > 0)
                    {            
                        for (int p = 0; p < ClusterSize; p++)
                        {
                            sum += clusters[i].getPoint(p).getVal(j);
                        }
                        clusters[i].setCentroidByPos(j, sum / ClusterSize);
                    }   
                }
            }

            //Sunfhkes gia termatismo tou range serach sth Cube
            if(count_points < 10 && epanalhjh > 25)
            {              
                //cout<<"Clustering teleiwse bash ths prwths sunfhkhs"<<count_points<<endl<<endl;
                break;
            }
            
            R = R*2;
            epanalhjh++;
            count_points=0;
        }

        //Topofethsh twn shmeiwn xwriss kentro
        for (int i = 0; i < all_points.size(); i++)
        {
            int currentClusterId = all_points[i].getCluster_ID();
            if(currentClusterId == 0)
                {
                    int nearestClusterId = getNearestClusterId(all_points[i]);
                    all_points[i].setCluster(nearestClusterId);
                    clusters[nearestClusterId-1].addPoint(all_points[i]);
                }
                
        }

        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = stop - start;

        //Silhouette

        long double sum_s=0;
        long double average=0;
        long double temp_dist;
        vector<long double> dist_s;
        vector<long double> si;
        vector<vector<long double>> si_k;

        //c.resize(a.size(), vector<double>((b.size())));
        si_k.resize(K);
        for (int q = 0; q < all_points.size(); q++)
        {
            
                        
            
            for (int n = 0; n < K; n++)
            {
                dist_s.push_back( euc_dist(clusters[n].getCentroid(),all_points[q].getCoord()));

            }       
            sort(dist_s.begin(), dist_s.end(), sort_using);
            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];
            //sum_s = sum_s + temp_dist;
            si_k[all_points[q].getCluster_ID()-1].push_back(temp_dist);

            //si.push_back(temp_dist);
            dist_s.clear();
            
        }
        long double av_si=0;
        for (int i1 = 0; i1 < K; i1++)
        {
            for (int i2 = 0; i2 < si_k[i1].size(); i2++)
            {
                av_si = av_si + si_k[i1][i2];
            }
            av_si = av_si/si_k[i1].size();
            si.push_back(av_si);
            average = average + temp_dist;
            av_si = 0;
        }
        average = average / K;

        //Eggrafh se arxeio
        ofstream outfile;
        outfile.open(output_dir);

        outfile<<"Algorithm: Hypercube"<<endl;

        if (outfile.is_open())
        {
            for (int i = 0; i < K; i++)
            {
                outfile << "CLUSTER-"<<clusters[i].get_clusterId()<<"{size: "<<clusters[i].getSize()<<  ", centroid : ";
                for (int j = 0; j < all_points[0].getDimensions(); j++)
                {
                    outfile<<" || "<< clusters[i].getCentroidByPos(j); // Output to file
                }
                outfile<<"}\n"<< endl;
            }

            outfile<<"clustering_time: "<<elapsed.count()<<endl<<endl;
            outfile<<"Silhouette: [";
            for (int i = 0; i < si.size(); i++)
            {
                outfile<<si[i]<<", ";
            }
            outfile<<" stotal: ";
            outfile<<average<<"]\n";

            if(c == 1){
                for (int i = 0; i < K; i++){
                    outfile << "\nCLUSTER-"<<clusters[i].get_clusterId()<<"{centroid";

                    for (int j = 0; j < clusters[i].getSize(); j++)
                    {
                        outfile<<", "<< (clusters[i].getPoint(j)).getID();
                    }

                    outfile << "}\n";
                }
            }
        }
        else
        {
            cout << "Error: Unable to write to clusters.txt";
        }
    }

static bool sort_using(double u, double v)
{
   return u < v;
}