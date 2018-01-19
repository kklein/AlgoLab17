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
#include <boost/graph/push_relabel_max_flow.hpp>
// Namespaces
using namespace std;
using namespace boost;

// Kevinkle: replaced setS by vecS in order to allow parallel edges.
typedef adjacency_list<vecS, vecS, undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		no_property,				// interior properties of vertices
		property<edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef property_map<Graph, edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Max flow definitions
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	FlowGraph;
// Interior Property Maps
typedef	boost::property_map<FlowGraph, boost::edge_capacity_t>::type		FlowEdgeCapacityMap;
typedef	boost::property_map<FlowGraph, boost::edge_residual_capacity_t>::type	FlowResidualCapacityMap;
typedef	boost::property_map<FlowGraph, boost::edge_reverse_t>::type		FlowReverseEdgeMap;
typedef	boost::graph_traits<FlowGraph>::vertex_descriptor			FlowVertex;
typedef	boost::graph_traits<FlowGraph>::edge_descriptor			FlowEdge;
typedef	boost::graph_traits<FlowGraph>::edge_iterator			FlowEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	FlowGraph &G;
	FlowEdgeCapacityMap	&capacitymap;
	FlowReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(FlowGraph & G, FlowEdgeCapacityMap &capacitymap, FlowReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		FlowEdge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void test_case() {
  // Create Graph, Vertices and Edges
  // ================================
  int n_nodes, n_edges, position_a, position_b, width, length;
  Vertex start, finish;
  Edge e;
  bool success;

  std::cin >> n_nodes >> n_edges >> start >> finish;

  std::vector<int> lengths(n_edges);
  std::vector<int> widths(n_edges);
  std::vector<std::vector<int> > endpoints(n_edges, std::vector<int>(2));

  Graph G(n_nodes);
  WeightMap weightmap = get(edge_weight, G);

  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    std::cin >> position_a >> position_b >> width >> length;
    tie(e, success) = add_edge(position_a, position_b, G);
    weightmap[e] = length;
    lengths[edge_index] = length;
    widths[edge_index] = width;
    endpoints[edge_index][0] = position_a;
    endpoints[edge_index][1] = position_b;
  }

  // Dijkstra shortest paths
  // =======================
  vector<Vertex> predmap_start(n_nodes);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
  vector<int> distmap_start(n_nodes);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
  dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
    predecessor_map(make_iterator_property_map(predmap_start.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
    distance_map(make_iterator_property_map(distmap_start.begin(), get(vertex_index, G))));	// distance map as Named Parameter

  // Dijkstra shortest paths
  // =======================
  vector<Vertex> predmap_finish(n_nodes);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
  vector<int> distmap_finish(n_nodes);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
  dijkstra_shortest_paths(G, finish, // We MUST provide at least one of the two maps
    predecessor_map(make_iterator_property_map(predmap_finish.begin(), get(vertex_index, G))).	// predecessor map as Named Parameter
    distance_map(make_iterator_property_map(distmap_finish.begin(), get(vertex_index, G))));	// distance map as Named Parameter

  assert(distmap_start[finish] == distmap_finish[start]);

  FlowGraph flow_graph;
  FlowEdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, flow_graph);
	FlowReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, flow_graph);
	FlowResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, flow_graph);
	EdgeAdder eaG(flow_graph, capacitymap, revedgemap);

  int shortest_distance = distmap_start[finish];
  // std::cout << "shortest distance: " << shortest_distance << std::endl;
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    int length = lengths[edge_index];
    FlowVertex position_a = endpoints[edge_index][0];
    FlowVertex position_b = endpoints[edge_index][1];
    if (distmap_start[position_a] + length + distmap_finish[position_b] ==
        shortest_distance) {
      eaG.addEdge(position_a, position_b, widths[edge_index]);
      // std::cout << "added edge to flow graph" << std::endl;
    } else if (distmap_start[position_b] + length +
        distmap_finish[position_a] == shortest_distance) {
      eaG.addEdge(position_b, position_a, widths[edge_index]);
    }
  }
  FlowVertex source = start;
  FlowVertex target = finish;
  long flow = boost::push_relabel_max_flow(flow_graph, source, target);
  std::cout << flow << std::endl;
}

// Main function looping over the testcases
int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
