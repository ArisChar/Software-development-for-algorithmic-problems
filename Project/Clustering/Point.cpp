#include "Point.h"

/*-------------------------Point_Class--------------------------*/

Point::Point(string line, int L){
    //pointId =0;
    values = lineToVec2(line , pointId);
    dimensions = values.size();
    clusterId = 0; // Initially not assigned to any cluster
    //cout<<"To id einai:"<<pointId<<endl;
    IDs.resize(L);
}

Point::Point(vector<double> cen,int cluid) {
    this->values = cen;
    dimensions = cen.size();
    this->clusterId = cluid;
    IDs.resize(100); //Gia apofhgh
}

vector<double> Point::lineToVec2(string &line ,int &id){

    vector<double> values;
    string tmp = "";
    bool id_ok = true;
    
    for (int i = 0; i < (int)line.length(); i++){

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' || line[i] == '+' || line[i] == '-' || line[i] == 'e'){
            tmp += line[i];

        }else if (tmp.length() > 0){

            if (id_ok == true){
                id = stoi(tmp);
                //cout<<"To id einai:"<<id<<endl;
                tmp = "";
                id_ok = false;
            }else{
                values.push_back(stod(tmp));
                tmp = "";
            }
        }
    }

    if (tmp.length() > 0){
        values.push_back(stod(tmp));
        tmp = "";
    }

    return values;
}

void Point::set_ID(long long int id, int pos) { IDs[pos]=id;}

long long int Point::get_ID(int pos) { return IDs[pos]; }

int Point::getDimensions() { return dimensions; }

int Point::getCluster() { return clusterId; }

int Point::getID() { return pointId; }

void Point::setCluster(int val) { clusterId = val; }

double Point::getVal(int pos) { return values[pos]; }

vector<double> Point::getCoord() { return values; }

void Point::setVal(int pos ,int val) { this->values[pos] = val;}

int Point::getCluster_ID() { return clusterId; }

void Point::print_point(){
    cout<<"To point me id "<<pointId <<" exei autes ths diastaseis:"<<endl;

    for (size_t i = 0; i < values.size(); i++){
        cout<<values[i]<<" ";
    }
        
}