#include<bits/stdc++.h>

using namespace std;

#define sz(x) (int)x.size()

// contsants
const int c = 1, k = 1; 

// take input of features and classes
void input(string file_name, vector<vector<int>> &feature_vectors) {
  ifstream in(file_name);
  string sample;
  while (!in.eof()) {
    getline(in, sample);
    vector<int> features;
    stringstream ss;
    ss << sample;
    int feature_value;
    while (ss >> feature_value) {
      features.push_back(feature_value);
    }
    feature_vectors.push_back(features);
  }
  in.close();
}

int main()
{
    vector<vector<int>>feature_vectors;
    input("training_set.txt",feature_vectors);

    int number_of_features = sz(feature_vectors[0]) - 1;

    vector<int>w(number_of_features+1);
    for(int i=0;i<number_of_features+1;++i)
    {
        w[i] = 0;
    }
    cerr << setfill('-') << setw(87) << "-" << endl;
    cerr<<"|t"<<setw(8)<<"|i"<<setw(8);
    for(int i=0;i<number_of_features;++i){
        cerr<<"|x"<<i<<setw(8);
    }
    cerr<<"|d"<<setw(8);
    for(int i=0;i<=number_of_features;++i){
        cerr<<"|Old w"<<i<<setw(8);
    }
    cerr<<"|D"<<setw(8)<<"|Error?"<<setw(8);
    for(int i=0;i<=number_of_features;++i){
        cerr<<"|New w"<<i<<setw(8);
    }
    cerr<<"|"<<endl;

    int count = 0, pass = 0;
    for(int iteration=1, index = 0;;++iteration,index++)
    {
        cerr << setfill(' ') << setw(87) << " " << endl;
        vector<int>old_w = w;
        int D = w[0];
        for(int i=1;i<=number_of_features;++i)
        {
            D += w[i] * feature_vectors[index][i-1];
        }
        bool isError = true;
        if(D < 0 && feature_vectors[index][number_of_features] < 0) isError = false;
        else if(D >= 0 && feature_vectors[index][number_of_features] >= 0) isError = false;

        if(isError){
            w[0] = w[0] + c * feature_vectors[index][number_of_features] * k;
            for(int i=1;i<=number_of_features;++i)
            {
                w[i] = w[i] + c * feature_vectors[index][number_of_features] * feature_vectors[index][i-1];
            }
        }

        cerr<<"|"<<iteration<<setw(8)<<index+1<<setw(8);
        for(int i=0;i<=number_of_features;++i){
            cerr<<feature_vectors[index][i]<<setw(8);
        }
        for(int i=0;i<=number_of_features;++i){
            cerr<<old_w[i]<<setw(8);
        }
        cerr<<D<<setw(8);
        cerr<<((isError) ? "Yes": "No")<<setw(8);
        for(int i=0;i<=number_of_features;++i){
            cerr<<w[i]<<setw(8);
        }
        cerr<<endl;

        if(!isError){
            count++;
        }
        else{
            pass++;
            count = 0;
        }
        if(count == sz(feature_vectors)){
            cerr<<"NUMBER OF ITERATIONS : "<<iteration<<endl;
            break;
        }
        if(index+1 == sz(feature_vectors)){
            index = -1;
        }
    }
    cerr << setfill('-') << setw(87) << "-" << endl;
    return 0;
}