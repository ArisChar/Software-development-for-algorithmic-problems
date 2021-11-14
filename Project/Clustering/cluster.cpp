#include "cluster.h"

Cluster::Cluster(int clusterId, Point centroid)
{
    this->clusterId = clusterId;
    this->Cluster_centroid = centroid.getCoord();
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

double Cluster::getCentroidByPos(int pos) { return Cluster_centroid[pos]; }

vector<double> Cluster::getCentroid() { return Cluster_centroid; }

void Cluster::setCentroidByPos(int pos, double val) { this->Cluster_centroid[pos] = val; }

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

        for (int q = 0; q < all_points.size(); q++)
        {
            for (int j = 0; j < K; j++)
            {
                dist_s.push_back( euc_dist(all_points[q].getCoord(),clusters[j].getCentroid()));
            }
            sort(dist_s.begin(), dist_s.end(), sort_using);

            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];

            si.push_back(temp_dist);

            sum_s = sum_s + temp_dist;

            dist_s.clear();
        }

        average = sum_s / all_points.size();        
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

        for (int q = 0; q < all_points.size(); q++)
        {
            for (int j = 0; j < K; j++)
            {
                dist_s.push_back( euc_dist(all_points[q].getCoord(),clusters[j].getCentroid()));
            }
            sort(dist_s.begin(), dist_s.end(), sort_using);

            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];

            si.push_back(temp_dist);

            sum_s = sum_s + temp_dist;

            dist_s.clear();
        }

        average = sum_s / all_points.size();

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

        for (int q = 0; q < all_points.size(); q++)
        {
            for (int j = 0; j < K; j++)
            {
                dist_s.push_back( euc_dist(all_points[q].getCoord(),clusters[j].getCentroid()));
            }
            sort(dist_s.begin(), dist_s.end(), sort_using);

            temp_dist = (dist_s[0] - dist_s[1]) / dist_s[1];

            si.push_back(temp_dist);

            sum_s = sum_s + temp_dist;

            dist_s.clear();
        }

        average = sum_s / all_points.size();

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