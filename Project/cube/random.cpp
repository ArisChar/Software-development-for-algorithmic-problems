#include "random.h"

double normal_distr(void){

    static random_device rd;
    mt19937 gen(rd()); 

    normal_distribution<double> d(0, 1); 

    return d(gen);
}

double uniform_distr(int w){

    static random_device rad;
    mt19937 gen(rad()); 

    uniform_real_distribution<double> d(0.0, (double) (w - 1));

    return d(gen);
}

int random_r(){

    static random_device rand_dev;
    mt19937 gen(rand_dev()); 

    uniform_real_distribution<float> ud(0, INT32_MAX);

    return round(ud(gen));
}
