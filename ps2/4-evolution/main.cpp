#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// #include "prettyprint.hpp"

void test_case() {
  int n_species, n_queries;
  std::cin >> n_species >> n_queries;
  std::vector<std::string> names(n_species);
  std::vector<int> ages(n_species), parents(n_species);
  std::map<std::string, int> index_loopup;
  std::string root_name;
  int max_age = 0;
  for (int i = 0; i < n_species; i++) {
    std::cin >> names[i] >> ages[i];
    index_loopup[names[i]] = i;
    if (ages[i] > max_age) {
      max_age = ages[i];
      root_name = names[i];
    }
  }

  for (int i = 0; i < n_species - 1; i++) {
    std::string child, parent;
    std::cin >> child >> parent;
    parents[index_loopup[child]] = index_loopup[parent];
  }
  parents[index_loopup[root_name]] = -1;

  for (int i = 0; i < n_queries; i++) {
    std::string target_name;
    int target_age;
    std::cin >> target_name >> target_age;
    if (target_age >= max_age) {
      std::cout << root_name << " ";
    } else {
      int previous_species;
      int current_species = index_loopup[target_name];
      while (ages[current_species] <= target_age and (current_species != -1)) {
        previous_species = current_species;
        current_species = parents[current_species];
      }
      std::cout << names[previous_species] << ' ';
    }
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
