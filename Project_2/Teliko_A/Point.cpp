#include "Point.h"

/*-------------------------Point_Class--------------------------*/
int Point::Geniko_id=0;

Point::Point(string line, int L, int metric){
    p_metric = metric;

    if(p_metric == 1){
        values = curveToVec(line);
        CreateTuple();
    }
    else{
        values = lineToVec2(line);
    }
    
    dimensions = values.size();
    clusterId = 0;
    IDs.resize(L);    
    Geniko_id++;
    pointId = Geniko_id;
}

vector<double> Point::lineToVec2(string &line){

    vector<double> values;
    string tmp = "";
    bool id_ok = true;
    
    for (int i = 0; i < (int)line.length(); i++)
    {

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.')
        {
            tmp += line[i];
        }
        else if (tmp.length() > 0)
        {
            values.push_back(stod(tmp));
            tmp = "";
        }
    }

    if (tmp.length() > 0){
        values.push_back(stod(tmp));
        tmp = "";
    }

    return values;
}

vector<double> Point::curveToVec(string &line){

    vector<double> values;
    string tmp = "";
    bool id_ok = true;
    int j = 1;
    
    for (int i = 0; i < (int)line.length(); i++)
    {

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.'){
            tmp += line[i];
        }
        else if (tmp.length() > 0)
        {
            values.push_back(j);
            values.push_back(stod(tmp));
            tmp = "";
            j++;
        }
    }

    if (tmp.length() > 0){
        values.push_back(j);
        values.push_back(stod(tmp));
        tmp = "";

    }

    return values;
}

void Point::CreateTuple(){
    curve.resize(values.size()/2, vector<double>((2)));
    for(int i=0; i<values.size(); i+=2){
        for (int j = 0; j < 2; j++){
            curve[i/2][j] = values[i+j];
        }
    }
}

void Point::setFiltered(vector<double>& data){
    for(int i=0; i< data.size(); i++){
        this->filtered.push_back(data[i]);
    }
}

void Point::EraseVal(int index){ values.erase(values.begin()+index); }

void Point::replace(vector<double>& c){ 
    values.clear();
    for(int i=0; i<c.size(); i++){
        values.push_back(c[i]);
    }
}

void Point::setCurve3(vector<double> vec){

    for(int i=0; i<vec.size(); i++){
        curve_3.push_back(vec[i]);
    }

}
vector<double> Point::getFiltered(){ return filtered; }

void Point::set_value(double value, int index){ values[index] = value; }

void Point::set_ID(long long int id, int pos) { IDs[pos]=id;}

long long int Point::get_ID(int pos) { return IDs[pos]; }

int Point::getDimensions() { return dimensions; }

int Point::getCluster() { return clusterId; }

int Point::getCluster_ID() { return clusterId; }

int Point::getID() { return pointId; }

void Point::setCluster(int val) { clusterId = val; }

double Point::getVal(int pos) { return values[pos]; }

vector<double> Point::getCoord() { return values; }

vector<double> Point::getCurve3() { return curve_3; }

vector<vector<double>> Point::getCurve() { return curve; }

void Point::print_point(){
    cout<<"To point me id "<<pointId <<" exei autes ths diastaseis:"<<endl;

    for (int i = 0; i < values.size(); i++){
        cout<<values[i]<<" ";
    }

}

