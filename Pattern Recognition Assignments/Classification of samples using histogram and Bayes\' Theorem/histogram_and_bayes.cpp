#include <bits/stdc++.h>

using namespace std;

#define sz(cont) (int)cont.size()

const float bucket_size = 1;

float p_of_A, p_of_B;

// Histogram of a class
void histogram(vector<int> &classX_histogram, vector<float> &classX) {
  float max_value = *max_element(classX.begin(), classX.end());
  int number_of_buckets = (int)ceil(max_value / bucket_size);
  classX_histogram.resize(number_of_buckets);
  for (auto value : classX) {
    int bucket = (int)floor(value / bucket_size);
    classX_histogram[bucket]++;
  }
}
// input values from files
void input(vector<float> &input_values, string from_file_name) {
  ifstream in(from_file_name);
  float value;
  while (in >> value) {
    input_values.push_back(value);
  }
}

int main() {
  // input value
  vector<float> classA, classB, samples;
  input(classA, "classA.txt");
  input(classB, "classB.txt");
  input(samples, "sample.txt");

  // class and sample sizes
  int classAcount = sz(classA), classBcount = sz(classB);

  // probability of class A and B
  p_of_A = classAcount / (float)(classAcount + classBcount);
  p_of_B = 1 - p_of_A;

  // calculate histogram
  vector<int> classA_histogram, classB_histogram;
  histogram(classA_histogram, classA);
  histogram(classB_histogram, classB);

  // for each sample calculate using bayes theorem
  for (auto sample : samples) {
    int bucket = (int)floor(sample / bucket_size);
    float pXA = classA_histogram[bucket] / (classAcount * bucket_size);
    float pXB = classB_histogram[bucket] / (classBcount * bucket_size);

    float pBayesA = (pXA * p_of_A) / (pXA * p_of_A + pXB * p_of_B);
    float pbayesB = 1 - pBayesA;

    cout<<"::: p(A|"<<sample<<") = "<<pBayesA<<", p(B|"<<sample<<") = "<<pbayesB<<endl;

    if (pBayesA > pbayesB) {
      cout << "::: Sample " << sample << " classified to class A :::" << endl;
    } else {
      cout << "::: Sample " << sample << " classified to class B :::" << endl;
    }
  }
  return 0;
}