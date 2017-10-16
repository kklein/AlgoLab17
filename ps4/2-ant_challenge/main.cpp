#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/strong_components.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

void test() {
  int n_trees, n_edges, n_species, start_tree, end_tree;
  cin >> n_trees >> n_edges >> n_species >> start_tree >> end_tree;
  for (int edge = 0; edge < n_edges; edge++) {
    int u, v, w;
    cin >> u >> v >> w;
  }
}

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test();
  }
  return 0;
}
