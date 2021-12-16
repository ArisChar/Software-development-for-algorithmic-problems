#include "Point.h"

/*-------------------------Point_Class--------------------------*/
int Point::Geniko_id=0;

Point::Point(string line, int L){
    //pointId =0;
    values = curveToVec(line);
    dimensions = values.size();
    CreateTuple();
    clusterId = 0; // Initially not assigned to any cluster
    //cout<<"To id einai:"<<pointId<<endl;
    IDs.resize(L);
    Geniko_id++;
    pointId=Geniko_id;
}

vector<double> Point::lineToVec2(string &line ,int &id){

    vector<double> values;
    string tmp = "";
    bool id_ok = true;
    
    for (int i = 0; i < (int)line.length(); i++)
    {

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' || line[i] == '+' || line[i] == '-')//|| line[i] == 'e'
        {
            tmp += line[i];
        }
        else if (tmp.length() > 0)
        {
        
            /*if (id_ok == true){
                //cout<<"edw lineToVec2 cpp"<<endl;
                //id = stoi(tmp);
                //id = Geniko_id;
                //cout<<"To id einai:"<<id<<endl;
                tmp = "";
                id_ok = false;
            }else{
                values.push_back(stod(tmp));
                tmp = "";
            }*/
            //cout<<tmp<<endl;
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

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' || line[i] == '+' || line[i] == '-'){
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

void Point::set_value(double value, int index){ values[index] = value; }

void Point::set_ID(long long int id, int pos) { IDs[pos]=id;}

long long int Point::get_ID(int pos) { return IDs[pos]; }

int Point::getDimensions() { return dimensions; }

int Point::getCluster() { return clusterId; }

int Point::getID() { return pointId; }

void Point::setCluster(int val) { clusterId = val; }

double Point::getVal(int pos) { return values[pos]; }

vector<double> Point::getCoord() { return values; }

vector<vector<double>> Point::getCurve() { return curve; }

void Point::print_point(){
    cout<<"To point me id "<<pointId <<" exei autes ths diastaseis:"<<endl;

    for (int i = 0; i < values.size(); i++){
        cout<<values[i]<<" ";
    }
    // cout<<endl;
    // cout<<endl;
    // cout<<"Hi"<<endl;
    // for (int i = 0; i < curve.size(); i++){
    //     for (int j = 0; j < curve[i].size(); j++){
            
    //         cout<<curve[i][j]<<"||";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl;
}

