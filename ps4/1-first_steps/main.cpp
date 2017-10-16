#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/strong_components.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef property_map<Graph, edge_weight_t>::type WeightMap;

void test() {
  int n_nodes, n_edges;
  cin >> n_nodes >> n_edges;
  Graph GT(n_nodes);
  WeightMap weightmap = get(edge_weight, GT);
  for (int edge = 0; edge < n_edges; edge++) {
    int u, v, w;
    cin >> u >> v >> w;
    Edge e; bool success;
    tie(e, success) = add_edge(v, u, GT);
    weightmap[e] = w;
  }
  vector<Edge> mst;
  kruskal_minimum_spanning_tree(GT, back_inserter(mst));

  int sum = 0;
  for(vector<Edge>::iterator it=mst.begin(); it != mst.end(); it++) {
    Edge e = *it;
    sum += weightmap[e];
  }

  vector<int> distances(n_nodes);
  vector<int> pred(n_nodes);
  int path_weight = 0;
  dijkstra_shortest_paths(GT, path_weight, predecessor_map(make_iterator_property_map(pred.begin(), get(vertex_index, GT))).
      distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, GT))));

  int max_distance = 0;
  for(int i = 0; i < n_nodes; i++) {
    if (distances[i] > max_distance) {
      max_distance = distances[i];
    }
  }

  cout << sum << ' ' << max_distance <<  endl;
}

int main() {
  int n_test_case;
  cin >> n_test_case;
  for (int test_case = 0; test_case < n_test_case; test_case++) {
    test();
  }
}
