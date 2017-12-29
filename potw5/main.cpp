#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <vector>

using namespace std;

bool compare(pair<int, int> p, pair<int, int> q) {
  return p.second > p.second;
}

void test() {
  int n_nodes, n_edges, n_rivers, origin, destination;
  cin << n_nodes << n_edges << n_rivers << origin << destination;
  vector<list<int>> adjacency(n_nodes, list<int>);
  int node1, node2, weight, is_river;
  for (int edge = 0; edge < n_edges; edge++) {
    cin >> node1 >> node2 >> weight >> is_river;
    adjacency[node1].push_back([node2, weight, is_river]);
    adjacency[node2].push_back([node1, weight, is_river]);
  }

  priority_queue<pair<int, int>, vector<int, int>, compare> q;
  for (int node = 0; node < n_nodes; node++) {
    if (node == origin) {
      q.push(make_pair(node, 0));
    } else {
      q.push(make_pair(node, numeric_limits<int>::max()));
    }
  }

}

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test();
  }
  return 0;
}
