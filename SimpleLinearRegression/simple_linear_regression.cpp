//Simple Linear Regression
//Using Ordinary Least Squares as sklearn does it.
/* The fit function is being given a totally preprocessed csv file, the functionality of pandas df like X.target selection
isn't been added here. So we're just assuming for ease that the first value is the predictor and the second is the target,
comma separated.*/
/*The train and test datasets used have been extracted from : https://www.kaggle.com/datasets/abhishek14398/salary-dataset-simple-linear-regression 
This is a Salary Dataset for Simple Linear Regression by Allena Venkata Sai Abhishek. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class LinearRegressor{
    private:
        double slope;
        double intercept;
        double mean_X;
        double mean_Y;
        double m;
        double c;
        void set_mean(string dataset);
    public:
        LinearRegressor(){
            mean_X=0;
            mean_Y=0;
            m=0;
            c=0;
        }
        
        void parameters(){
            cout<<"Mean of the independent variable: "<<mean_X<<endl;
            cout<<"Mean of the dependent variable: "<<mean_Y<<endl;
            cout<<"Slope of the best fit line: "<<m<<endl;
            cout<<"Intercept of the best fit line: "<<c<<endl;
        }
        void fit(string dataset);
        vector<double> predict(string dataset);
        
        
};

void LinearRegressor :: set_mean(string dataset){

    ifstream file(dataset);
    if (!file.is_open()) {
        cerr << "Failed to open file" << endl;
    }

    std::string headerLine;
    std::getline(file, headerLine);

    string line;
    double x;
    double target;

    int i=0;
    while (std::getline(file, line)) {
        i++;
        std::stringstream ss(line); 
        std::string predictor; 
        std::string targetStr;       

        if (std::getline(ss, predictor, ',')) {
            if (getline(ss, targetStr, ',')) {
                    x = stod(predictor); 
                    mean_X += x;
                    target = stod(targetStr);
                    mean_Y+=target;
            }
        }
    }
        mean_X = mean_X/i;
        mean_Y = mean_Y/i;
    file.close();
}

void LinearRegressor :: fit(string dataset){
    set_mean(dataset);
    double var = 0;

    ifstream file(dataset);
    if (!file.is_open()) {
        cerr << "Failed to open file" << endl;
    }

    std::string headerLine;
    std::getline(file, headerLine);

    string line;
    double x;
    double target; 

    while (std::getline(file, line)) {

        std::stringstream ss(line); 
        std::string predictor; 
        std::string targetStr;      

        if (std::getline(ss, predictor, ',')) {
            if (getline(ss, targetStr, ',')) {
                    x = stod(predictor); 
                    target = stod(targetStr);
                    m+=(x-mean_X)*(target-mean_Y);
                    var+=pow(x-mean_X,2);
            }
        }
    } m=m/var;
    c=mean_Y-m*mean_X;
    file.close();
}

vector<double> LinearRegressor :: predict(string dataset){

    ifstream file(dataset);
    if (!file.is_open()) {
        cerr << "Failed to open file" << endl;
    }

    std::string headerLine;
    std::getline(file, headerLine);

    string line;
    double x;

    vector<double> y_pred;
    while (std::getline(file, line)) {
        std::stringstream ss(line); 
        std::string predictor; 
        std::string targetStr;       

        if (std::getline(ss, predictor, ',')) {
                    x = stod(predictor); 
                    y_pred.push_back(m*x+c);
        }
    }
    file.close();
    return y_pred;
}

class Metrics{
    private:
        double mse;
        double rmse;
    public:

        Metrics(){
            mse=0;
            rmse=0;
        };
        double MSE(string dataset, vector<double> y_pred);
        double RMSE(string dataset, vector<double> y_pred){
            mse=MSE(dataset,y_pred);
            return rmse;
        }
};

double Metrics :: MSE(string dataset, vector<double> y_pred){
    double mse = 0;

    ifstream file(dataset);
    if (!file.is_open()) {
        cerr << "Failed to open file" << endl;
    }

    std::string headerLine;
    std::getline(file, headerLine);

    string line;
    int i=0;
    double target;
    while (std::getline(file, line)) {

        std::stringstream ss(line); 
        std::string predictor; 
        std::string targetStr;       

        if (std::getline(ss, predictor, ',')) {
            if (getline(ss, targetStr, ',')) {
                    target = stod(targetStr);
                    mse+=pow((target-y_pred[i]),2);
            }
        } i++;
    } 
    mse= mse/i;
    rmse = pow(mse,0.5);
    return mse;
}

int main(){

    string filename = "train.csv";
    vector<double> y_pred;

    LinearRegressor model;
    model.fit(filename);
    model.parameters();

    y_pred=model.predict("test.csv");
    cout<<"Predicted values:"<<endl;
    for(int val:y_pred){
        cout<<val<<" ";
    }
    cout<<"\n";

    Metrics metrics;
    cout<<"Mean Squared Error (MSE): ";
    cout<<metrics.MSE("test.csv",y_pred)<<endl;
    cout<<"Root Mean Squared Error (RMSE): ";
    cout<<metrics.RMSE("test.csv",y_pred);


    return 0;
}
