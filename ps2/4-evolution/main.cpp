#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

int bs(int target_age, std::vector<int> &path, std::vector<int> &ages) {
  int l_min = -1;
  int l_max = path.size() - 1;
  while (l_min + 1 < l_max) {
    int l = (l_max + l_min) / 2;
    if (ages[path[l]] <= target_age) {
      l_max = l;
    } else {
      l_min = l;
    }
  }
  return path[l_max];
}

void dfsbs(int species, std::vector<int> &path,
    std::vector<std::vector<std::pair<int, int> > > &queries,
    std::vector<int> &results, std::vector<std::vector<int> > &children, std::vector<int> &ages) {
  for (std::pair<int, int> query: queries[species]) {
    results[query.second] = bs(query.first, path, ages);
  }
  for (int child: children[species]) {
    path.push_back(child);
    dfsbs(child, path, queries, results, children, ages);
  }
  path.pop_back();
}

void test_case() {
  int n_species, n_queries, root_index;
  std::cin >> n_species >> n_queries;
  std::vector<std::string> names(n_species);
  std::vector<int> ages(n_species);
  std::vector<std::vector<int> > children(n_species);
  std::unordered_map<std::string, int> index_lookup;
  std::string root_name;
  int max_age = 0;
  for (int i = 0; i < n_species; i++) {
    std::cin >> names[i] >> ages[i];
    index_lookup[names[i]] = i;
    if (ages[i] > max_age) {
      max_age = ages[i];
      root_name = names[i];
    }
  }
  root_index = index_lookup[root_name];

  for (int i = 0; i < n_species - 1; i++) {
    std::string child, parent;
    std::cin >> child >> parent;
    int child_index = index_lookup[child];
    int parent_index = index_lookup[parent];
    children[parent_index].push_back(child_index);
  }

  std::vector<std::vector<std::pair<int, int> > > queries(n_species);
  std::vector<int> results(n_queries);
  for (int i = 0; i < n_queries; i++) {
    std::string target_name;
    int target_age;
    std::cin >> target_name >> target_age;
    queries[index_lookup[target_name]].push_back(std::make_pair(target_age, i));
  }

  std::vector<int> path;
  path.push_back(root_index);
  dfsbs(root_index, path, queries, results, children, ages);

  for (int i = 0; i < n_queries; i++) {
    std::cout << names[results[i]];
    if (i < n_queries - 1) {
      std::cout << " ";
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
