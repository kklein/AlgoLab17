#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  int n_nodes, n_edges, n_queries, longitude, latitude, u, v, weight;
  cin >> n_nodes >> n_edges >> n_queries;
  for (int node_index = 0; node_index < n_nodes; ++node_index) {
    cin >> longitude >> latitude;
  }
  for (int edge_index = 0; edge_index < n_edges; ++edge_index) {
    cin >> u >> v >> weight;
  }
  for (int query_index = 0; query_index < n_queries; ++query_index) {
    cin >> u >> v;
  }
  return 0;
}
