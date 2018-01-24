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
#include <stack>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


// Run a DFS and determine for each vertex the largest edge weight on path from
// root to vertex.
std::vector<int> get_heaviest_edge_on_path(Graph &G, WeightMap &weightmap,
    int n_vertices, Vertex root) {
  std::vector<bool> visited(n_vertices, false);
  std::stack<Vertex> to_visit;
  std::vector<int> largest_weight(n_vertices);
  visited[root] = true;
  to_visit.push(root);
  largest_weight[root] = 0;
  OutEdgeIt oebeg, oeend;
  while (!to_visit.empty()) {
    Vertex current_vertex = to_visit.top();
    to_visit.pop();
    for (boost::tie(oebeg, oeend) = boost::out_edges(current_vertex, G);
        oebeg != oeend; ++oebeg) {
      Vertex u = boost::source(*oebeg, G);
      Vertex v = boost::target(*oebeg, G);
      if (visited[v]) {
        continue;
      }
      int w = weightmap[*oebeg];
      largest_weight[v] = std::max(w, largest_weight[current_vertex]);
      visited[v] = true;
      to_visit.push(v);
    }
  }
  return largest_weight;
}

void testcase() {
  int n_planets, start_index, weight;
  Edge e; bool success;
  std::cin >> n_planets >> start_index;
  // Input indeces are off by 1.
  start_index--;
  Vertex start = start_index;
	Graph G(n_planets);
  // TODO(kevinkle): See todo below.
  std::vector<std::vector<int> >
      weights(n_planets, std::vector<int>(n_planets, -1));
	WeightMap weightmap = boost::get(boost::edge_weight, G);
  std::map<Edge, bool> edge_taken;
	for (int planet_a = 0; planet_a < n_planets - 1; planet_a++) {
    for (int planet_b = planet_a + 1; planet_b < n_planets; planet_b++) {
      std::cin >> weight;
      weights[planet_a][planet_b] = weights[planet_b][planet_a] = weight;
  		boost::tie(e, success) = boost::add_edge(planet_a, planet_b, G);	// Adds edge from u to v. If parallel edges are allowed, success is always true.
      assert(success == true);
  		weightmap[e] = weight;
      edge_taken[e] = false;
    }
	}

  // A graph consisting of Prim's MST.
  Graph G2(n_planets);
  WeightMap weightmap2 = boost::get(boost::edge_weight, G2);

	std::vector<Vertex> primpredmap(n_planets);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
			boost::root_vertex(start));									// old primpredmap gets reset first!
	int totalweight = 0;
	for (int i = 0; i < n_planets; ++i) {
		if (primpredmap[i] != i) {
      Edge e; bool success;
      int w;
      boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
      w = weightmap[e];
      totalweight += weightmap[e];
      edge_taken[e] = true;
      boost::tie(e, success) = boost::add_edge(i, primpredmap[i], G2);
      weightmap2[e] = w;
    }
  }

  Edge desired_edge;
  int min_weight_gain = INT_MAX;
  // Caching - crucial for performance.
  std::vector<std::vector<int> >
      heaviest_edges(n_planets, std::vector<int> (n_planets, - 1));
  // Inspect all edges that have not been part of the MST.
  for (auto const& x: edge_taken) {
    if (!x.second) {
      desired_edge = x.first;
      Vertex candidate_vertex_1 = boost::source(desired_edge, G);
      Vertex candidate_vertex_2 = boost::target(desired_edge, G);
      // TODO(kevinkle): Figure out why this commented piece of code does not
      // work instead of using the 'weights' vector.
      // int desired_edge_weight = weightmap[e];
      int desired_edge_weight = weights[candidate_vertex_1][candidate_vertex_2];
      if (heaviest_edges[candidate_vertex_1][candidate_vertex_2] == -1) {
        heaviest_edges[candidate_vertex_1] = get_heaviest_edge_on_path(G2,
            weightmap2, n_planets, candidate_vertex_1, candidate_vertex_2);
      }
      int candidate_weight =
          heaviest_edges[candidate_vertex_1][candidate_vertex_2];
      min_weight_gain = std::min(min_weight_gain, desired_edge_weight
          - candidate_weight);
    }
  }

  std::cout << totalweight + min_weight_gain << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int n_test_cases;
  std::cin >> n_test_cases;
	for (int test_index = 0; test_index < n_test_cases; test_index++) {
    testcase();
  }
	return 0;
}
