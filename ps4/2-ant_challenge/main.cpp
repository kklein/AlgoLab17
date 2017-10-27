#include <boost/graph/adjacency_list.hpp>
#include <boost/config.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/strong_components.hpp>
#include <iostream>
#include <limits>
#include <vector>

using namespace boost;
using namespace std;

// Replace the 'standard' edge_weight_t property by edge_index_t property.
typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_index_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

void test() {
  int n_trees, n_edges, n_species, start_tree, end_tree;
  cin >> n_trees >> n_edges >> n_species >> start_tree >> end_tree;
  Graph graph(n_species);
  property_map<Graph, edge_index_t>::type edge_indices = get(edge_index, graph);
  vector<vector<int> > weights(n_species, vector<int>(n_edges));
  Edge edge;
  int u, v, w;
  bool success;
  vector<int> hives(n_species);
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    cin >> u >> v;
    tie(edge, success) = add_edge(u, v, graph);
    edge_indices[edge] = edge_index;
    for (int species = 0; species < n_species; species++) {
      cin >> weights[species][edge_index] ;
    }
  }
  vector<vector<Edge> > msts();
  for (int species = 0; species < n_species; species++) {
    cin >> hives[species];
  }
  vector<int> overall_tree_weights(n_edges, numeric_limits<int>::max());
  for (int species = 0; species < n_species; species++) {
    vector<Edge> mst;
    kruskal_minimum_spanning_tree(graph, back_inserter(mst),
        weight_map(make_iterator_property_map(weights[species].begin(), edge_indices)));
    for(vector<Edge>::iterator spanning_tree_edge = mst.begin(); spanning_tree_edge != mst.end(); ++spanning_tree_edge) {
      if (overall_tree_weights[edge_indices[*spanning_tree_edge]] > weights[species][edge_indices[*spanning_tree_edge]]) {
        overall_tree_weights[edge_indices[*spanning_tree_edge]] = weights[species][edge_indices[*spanning_tree_edge]];
      }
    }
  }
  vector<Vertex> predecessors(n_trees);	//
  vector<int> distances(n_trees);
  dijkstra_shortest_paths(graph, start_tree,
      predecessor_map(&predecessors[0]).
      distance_map(&distances[0]).
      weight_map(make_iterator_property_map(&overall_tree_weights[0], edge_indices)));
  cout << distances[end_tree] << endl;
}

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test();
  }
  return 0;
}
