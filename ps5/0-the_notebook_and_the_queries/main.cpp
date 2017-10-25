#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

void test() {
  int n_parameters, n_experiments, n_queries, lower_bound, upper_bound;
  cin >> n_parameters >> n_experiments >> n_queries;
  // This implementation ignores the bounds.
  for (int param_index = 0; param_index < n_parameters; ++param_index) {
    cin >> lower_bound;
    cin >> upper_bound;
  }
  unordered_map<vector<int>, int, boost::hash<vector<int> > > results;
  vector<int> parameters(n_parameters);
  for (int experiment_index = 0; experiment_index < n_experiments; ++experiment_index) {
    for (int param_index = 0; param_index < n_parameters; ++param_index) {
      cin >> parameters[param_index];
    }
    cin >> results[parameters];
  }
  vector<int> query(n_parameters);
  for (int query_index = 0; query_index < n_queries; ++query_index) {
    for (int param_index = 0; param_index < n_parameters; ++param_index) {
      cin >> query[param_index];
    }
    if (results.find(query) == results.end()) {
      cout << "-1" << endl;
    } else {
      cout << results[query] << endl;
    }
  }
}

int main(){
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; ++test_case) {
    test();
  }
  return 0;
}
