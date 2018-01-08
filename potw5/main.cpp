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
// #include <boost/graph/prim_minimum_spanning_tree.hpp>
// #include <boost/graph/kruskal_min_spanning_tree.hpp>
// #include <boost/graph/connected_components.hpp>
// #include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


void update_weight(WeightMap &weightmap, Edge e, bool success, int weight) {
  if (!success) {
    // Edge already existed.
    weightmap[e] = min(weightmap[e], weight);
  } else {
    weightmap[e] = weight;
  }
}

void testcase() {
  int n_cities, n_edges, n_req_rivers, origin, destination;
  cin >> n_cities >> n_edges >> n_req_rivers >> origin >> destination;
  // V = cities X {0,...,n_req_rivers}
  // Each node in the graph represents a city and a number of rivers visited
  // thus far.
  // City with 4 visited rivers is access via
  // (1 + n_req_rivers) * city_index + 4
  Graph G((1 + n_req_rivers) * n_cities);
  WeightMap weightmap = get(edge_weight, G);
  int location_a, location_b, weight, is_river;
  Edge e;
  bool success;
  
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    cin >> location_a >> location_b >> weight >> is_river;
    // We can just as well an edge from (location_a, river_count) to
    // (location_b, river_count) if the edge is a river - this will only
    // increase the number of visited rivers.
    for (int river_count = 0; river_count <= n_req_rivers; river_count++) {
      tie(e, success) = add_edge(
          (1 + n_req_rivers) * location_a + river_count,
          (1 + n_req_rivers) * location_b + river_count, G);
      update_weight(weightmap, e, success, weight);
    }
    if (is_river == 1) {
      for (int river_count = 0; river_count < n_req_rivers; river_count++) {
        tie(e, success) = add_edge(
            (1 + n_req_rivers) * location_a + river_count,
            (1 + n_req_rivers) * location_b + river_count + 1, G);
        update_weight(weightmap, e, success, weight);
        tie(e, success) = add_edge(
          (1 + n_req_rivers) * location_b + river_count,
          (1 + n_req_rivers) * location_a + river_count + 1, G);
        update_weight(weightmap, e, success, weight);
      }
    }
  }

  int total_distance = INT_MAX;

  // Dijkstra shortest paths
  // =======================
  vector<Vertex> predmap((1 + n_req_rivers) * n_cities);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	vector<int> distmap((1 + n_req_rivers) * n_cities);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = (1 + n_req_rivers) * origin;
	dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter

  cout << distmap[(1 + n_req_rivers) * destination + n_req_rivers] << endl;
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
