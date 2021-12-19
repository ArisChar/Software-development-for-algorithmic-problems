#include "h_hash.h"

/*-------------------------h_hash--------------------------*/

h_hash::h_hash(int dim){
    this->w = 100;   // We set w to 100
    this->dim = dim;

    // Get dim random values and store it to v to get the random vector
    for(int i=0; i<dim; i++){
        v.push_back(normal_distr()); 
    }

    // Give t random value
    t = uniform_distr(w);
}

long long int h_hash::hash_function(Point& point){
    // Get coordinates of point
    vector<double> p = point.getCoord();

    // Calculate inner product of p with v
    double product = inner_product(p, this->v);

    // Add t to product and store it to sum
    double sum = product + this->t;

    // Divide by w 
    double h = sum / double (this->w);

/*    
    for(int i=0; i<this->dim; i++){
        cout<<p[i]<<"||"<<this->v[i]<<endl;
    }
    cout<<"Product:"<< product<<endl;
    cout<<"t is:"<<this->t<<endl;
    cout<<"sum:"<<sum<<endl;
    cout<<"h is:"<<h<<endl;
*/

    // Return the floor of h which is h(p)
    return floor(h);
}

double h_hash::inner_product(vector<double> x, vector<double> y){

    if(x.size() != y.size()) {
        //puts( "Error a's size not equal to b's size" ) ;
        return -1 ;
    }

    double product = 0;

    // multiplies each individual coordinate of each other
    for (int i = 0; i <= x.size()-1; i++){
        product += (x[i])*(y[i]); 
    }

    return product;
}

vector<double> h_hash::get_v() { return v; }

int h_hash::get_t(){ return t; }

int h_hash::get_w(){ return w; }
