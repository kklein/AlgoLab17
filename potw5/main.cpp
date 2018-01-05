// ALGOLAB BGL Tutorial 1
// Code snippets demonstrating
// - graph definitions
// - several algorithms (components, distance-based algorithms, maximum matching)
// - how to pass exterior property maps
// - use of iterators

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-code_snippets.cpp -o bgl-code_snippets; ./bgl-code_snippets

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Functions
// =========
void testcase() {
  int n_nodes, n_edges, n_rivers, origin, destination;
  cin >> n_nodes >> n_edges >> n_rivers >> origin >> destination;
  Graph G(2 * n_nodes);
  WeightMap weightmap = get(edge_weight, G);
  int location_a, location_b, weight, is_river;
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    Edge e;	bool success;
    cin >> location_a >> location_b >> weight >> is_river;
    if (is_river == 1) {
      tie(e, success) = add_edge(2 * location_a, 2 * location_b + 1, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_a + 1, 2 * location_b + 1, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_b, 2 * location_a + 1, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_b + 1, 2 * location_a + 1, G);
      weightmap[e] = weight;
    } else {
      tie(e, success) = add_edge(2 * location_a, 2 * location_b, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_a + 1, 2 * location_b + 1, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_b, 2 * location_a, G);
      weightmap[e] = weight;
      tie(e, success) = add_edge(2 * location_b + 1, 2 * location_a + 1, G);
      weightmap[e] = weight;
    }
  }

  int total_distance = INT_MAX;

  // Dijkstra shortest paths
  // =======================
  vector<Vertex> predmap(2 * n_nodes);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	vector<int> distmap(2 * n_nodes);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = 2 * origin;
	dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter

  cout << distmap[2 * destination + 1] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int n_test_cases;
  cin >> n_test_cases;
	for (int test_case_index = 0; test_case_index < n_test_cases; test_case_index++) {
    testcase();
  }
	return 0;
}
