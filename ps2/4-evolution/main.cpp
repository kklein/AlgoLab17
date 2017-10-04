#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_species;
    int n_queries;
    cin >> n_species;
    cin >> n_queries;
    map<string, int> ages;
    map<string, string> parents;
    string name;
    int max_age = 0;
    string root_name;
    for (int i = 0; i < n_species; i++) {
      cin >> name;
      cin >> ages[name];
      if (ages[name] > max_age) {
        max_age = ages[name];
        root_name = name;
      }
    }
    for (int i = 0; i < n_species - 1; i++) {
      cin >> name;
      cin >> parents[name];
    }
    parents[root_name] = "stop";
    for (int i = 0; i < n_queries; i++) {
      string target_name;
      cin >> target_name;
      int target_age;
      cin >> target_age;
      if (target_age >= max_age) {
        cout << root_name << " ";
      } else {
        string previous_name;
        string current_name = target_name;
        while (ages[current_name] <= target_age and (current_name != "stop")) {
          previous_name = current_name;
          current_name = parents[current_name];
        }
        cout << previous_name << ' ';
      }
    }
    cout << endl;
  }
  return 0;
}
