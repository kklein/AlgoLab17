// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
// Namespaces
// using namespace std;
// using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {
  int n_locations, n_paths, n_soldiers, n_soldiers_min, n_soldiers_max,
      location_a, location_b;
  int total_n_soldiers_min = 0;
  int total_n_soldiers_min_eddges = 0;
  std::cin >> n_locations >> n_paths;

  // Create Graph and Maps
  Graph G(n_locations);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, revedgemap);

  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  for (int location_index = 0; location_index < n_locations; location_index++) {
    std::cin >> n_soldiers >> n_soldiers_min;
    eaG.addEdge(source, location_index, n_soldiers);
    eaG.addEdge(location_index, target, n_soldiers_min);
    total_n_soldiers_min += n_soldiers_min;
  }
  for (int path_index = 0; path_index < n_paths; path_index++) {
    std::cin >> location_a >> location_b >> n_soldiers_min >> n_soldiers_max;
    eaG.addEdge(location_a, location_b, n_soldiers_max - n_soldiers_min);
    eaG.addEdge(source, location_b, n_soldiers_min);
    eaG.addEdge(location_a, target, n_soldiers_min);
    total_n_soldiers_min_eddges += n_soldiers_min;
  }

  long flow = boost::push_relabel_max_flow(G, source, target);
  if (flow >= total_n_soldiers_min + total_n_soldiers_min_eddges) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }

}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_case_index = 0; test_case_index < n_test_cases; test_case_index++) {
    testcases();
  }
  return 0;
}
