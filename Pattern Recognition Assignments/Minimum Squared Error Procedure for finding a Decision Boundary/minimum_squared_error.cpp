#include <bits/stdc++.h>

using namespace std;

#define sz(x) (int)x.size()
#define DBG(x) cout<<#x<<" : "<<x<<endl

// single equation element with coefficient
struct EquationElement {
  vector<int> elements, squared;
  int coefficient,extraa;
  int sign = 1;
  EquationElement() {}
  EquationElement(vector<int> elements_given, vector<int> squared_given,
                  int coefficient_given, int sign_given) {
    elements = elements_given;
    squared = squared_given;
    coefficient = coefficient_given;
    sign = sign_given;
    extraa = 0;
  }
  void toString() {
    if (sign > 0)
      cout << " + ";
    else
      cout << " - ";
    if (coefficient > 1) {
      cout << coefficient << "*";
    }
    for (int i = 0; i < sz(elements); ++i) {
      cout << "w" << elements[i];
      if (squared[i] > 1)
        cout << "^" << squared[i];
    }
  }
};
// upto three features
// set the possible equation
void set_equation(vector<EquationElement> &equation, int number_of_features,
                  int sign) {
  // a(1)^2 + .. + a(n)^2
  for (int i = 0; i <= number_of_features; ++i) {
    vector<int> elements, squared;
    elements.push_back(i);
    squared.push_back(2);
    equation.push_back(EquationElement(elements, squared, 1, 1));
  }
  for (int i = 0; i < number_of_features; ++i) {
    for (int j = i + 1; j <= number_of_features; ++j) {
      vector<int> elements, squared;
      elements.push_back(i);
      elements.push_back(j);
      squared.push_back(1);
      squared.push_back(1);
      equation.push_back(EquationElement(elements, squared, 2, 1));
    }
  }
  for (int i = 0; i <= number_of_features; ++i) {
    vector<int> elements, squared;
    elements.push_back(i);
    squared.push_back(1);
    equation.push_back(EquationElement(elements, squared, 2, sign));
  }
  vector<int> elements, squared;
  equation.push_back(EquationElement(elements, squared, 1, 1));
}
// square equation elements
// (a+b)^2 = a^2 + 2*a*b + b^2
tuple<EquationElement, EquationElement, EquationElement>
square(EquationElement a, EquationElement b) {
  EquationElement a2, ab2, b2;
  // 2ab
  int ab2_coeeficient = 2 * a.coefficient * b.coefficient;

  for (int i = 0; i < sz(a.squared); ++i) {
    a.squared[i] *= 2;
  }
  for (int i = 0; i < sz(b.squared); ++i) {
    b.squared[i] *= 2;
  }
  return make_tuple(a2, ab2, b2);
}
// calculate E
// every equation has a power of two - assumed in coding
void sumOfSquaredErrors(vector<vector<int>> &feature_vectors,
                        vector<vector<EquationElement>> &E) {
  for (auto feature_vector : feature_vectors) {
    vector<EquationElement> equation;
    vector<int> elements, squared;
    int feature_vector_size = sz(feature_vector);
    squared.push_back(1);
    elements.push_back(0);
    equation.push_back(EquationElement(elements, squared, 1, 1));
    for (int i = 0; i < feature_vector_size - 1; ++i) {
      squared.clear();
      elements.clear();
      if (feature_vector[i]) {
        squared.push_back(1);
        elements.push_back(i + 1);
        equation.push_back(
            EquationElement(elements, squared, feature_vector[i], 1));
      }
    }
    squared.clear();
    elements.clear();
    equation.push_back(EquationElement(
        elements, squared, feature_vector[feature_vector_size - 1], -1));
    E.push_back(equation);
  }
}
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

int main() {
  vector<vector<int>> feature_vectors;
  input("training_set.txt", feature_vectors);

  vector<vector<EquationElement>> E;
  // sumOfSquaredErrors(feature_vectors,E);

  int total_features = sz(feature_vectors[0]) - 1;
  int sample_size = sz(feature_vectors);
  vector<EquationElement> Equation;
  set_equation(Equation, total_features, -1);
  for(int i=0;i<sz(Equation);++i)
  {
    //   cerr<<i<<" "<<Equation[i].coefficient<<endl;
      Equation[i].coefficient *= sample_size;
    //   cerr<<i<<" "<<Equation[i].coefficient<<endl;
      //Equation[i].toString();
  }
  vector<vector<EquationElement>>equations;
  int id = 0;
  for (auto x : feature_vectors) {
    vector<EquationElement>equation;
    int features = sz(x);
    equation.push_back(Equation[0]);
    for(int j=1;j<sz(Equation)-1;++j)
    {
        if(j <= total_features) id = 1;
        else id = 2;
        cout<<"BEGIN : "; Equation[j].toString(); cout<<endl;
        int found = 0, num_elements = sz(Equation[j].elements);
        int have_count=0;
        vector<int>points;
        int yes = 0;
        for(int k=0;k<num_elements;++k)
        {
            if(Equation[j].elements[k] == 0) yes = 1;
            for (int i = 0; i < features-1; ++i) {
                cerr<<"INNER "<<x[i] << " " << Equation[j].elements[k] << " "<< i+1<<endl;
                if(x[i] && Equation[j].elements[k] == i+1)
                {
                    points.push_back(x[i]);
                    have_count++;
                    break;
                }
            }
        }
        if(yes) num_elements--;
        cerr<<j<<" "<<have_count<<" "<<num_elements<<endl;
        if(have_count != num_elements)
        {
            
            int unit_value = id; // culprit
            cout<<"CUT : "<<unit_value<<endl;
            Equation[j].coefficient -= unit_value;
            for(auto point : points)
            {
                Equation[j].extraa += (unit_value * point) - unit_value; 
            }
        }
        cout<<"END : ";Equation[j].toString();
        cout<<endl;
    }
    id++;
  }
  DBG(sz(Equation));
  for(auto x : Equation)
  {
      x.toString(); cout<<" ";
  }
  return 0;
}