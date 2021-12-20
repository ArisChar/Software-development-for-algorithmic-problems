#include "Point.h"
int Point::Geniko_id=0;
/*-------------------------Point_Class--------------------------*/

vector <double> tuble_to_vector(vector<vector<double>> a)
{
        vector <double> temp;

        for (size_t i = 0; i < a.size(); i++)
        {
                for (size_t j = 0; j < 2; j++)
                {
                        temp.push_back(a[i][j]);
                }
                
        }
        

        return temp;
}

//Gia metrikh L2
Point::Point(string line, int L){
    //pointId =0;
    values = lineToVec2(line , pointId);
    dimensions = values.size();
    clusterId = 0; // Initially not assigned to any cluster
    //cout<<"To id einai:"<<pointId<<endl;
    IDs.resize(L);
    Geniko_id++;
    pointId=Geniko_id;
}

//Gia to clustering
Point::Point(vector<double> cen,int cluid) {
    this->values = cen;
    dimensions = cen.size();
    this->clusterId = cluid;
    IDs.resize(100); //Gia apofhgh
}

//Gia to clustering DFD
Point::Point(vector<vector<double>> cen,int cluid,int metrikh) {
    this->curve = cen;
    this->values = tuble_to_vector(cen);
    dimensions = values.size();
    
    this->clusterId = cluid;
    IDs.resize(100); //Gia apofhgh
}

//Gia thn DFD
Point::Point(string line, int L,int metrikh){
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

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' )//|| line[i] == 'e'|| line[i] == '+' || line[i] == '-'
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

        if ((48 <= int(line[i]) && int(line[i])  <= 57) || line[i] == '.' ){//|| line[i] == '-'|| line[i] == '+' 
            tmp += line[i];
        }
        else if (tmp.length() > 0)
        {
            values.push_back(j);
            //cout<<"stod(tmp): "<<tmp<<endl;
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


int Point::getID() { return pointId; }

void Point::setCluster(int val) { clusterId = val; }

double Point::getVal(int pos) { return values[pos]; }

vector<double> Point::getCoord() { return values; }

void Point::setVal(int pos ,int val) { this->values[pos] = val;}

int Point::getCluster_ID() { return clusterId; }

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

