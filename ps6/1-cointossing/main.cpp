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
void testcase() {
  // std::cout << "Start testcase." << std::endl;
  int n_players, n_rounds, player_a, player_b, outcome;
  int desired_flow = 0;
  int desired_capacity = 0;
  int n_outstanding_matches = 0;

  bool flag = false;
  std::cin >> n_players >> n_rounds;
  std::vector<int> scores(n_players, 0);
  std::vector<int> desired_scores(n_players, 0);

  // Create Graph and Maps
	Graph G(n_players);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

  Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);

  for (int round_index = 0; round_index < n_rounds; round_index++) {
    std::cin >> player_a >> player_b >> outcome;
    switch (outcome) {
      case 0: {
        n_outstanding_matches++;
        Vertex match = boost::add_vertex(G);
        eaG.addEdge(source, match, 1);
        eaG.addEdge(match, player_a, 1);
        eaG.addEdge(match, player_b, 1);
        break;
      }
      case 1: {
        scores[player_a]++;
        break;
      }
      case 2: {
        scores[player_b]++;
        break;
      }
    }
  }
  for (int player_index = 0; player_index < n_players; player_index++) {
    std::cin >> desired_scores[player_index];
    if (desired_scores[player_index] < scores[player_index]) {
      flag = true;
    }
    desired_capacity = desired_scores[player_index] - scores[player_index];
    desired_flow += desired_capacity;
    eaG.addEdge(player_index, target, desired_capacity);
  }

	long flow = boost::push_relabel_max_flow(G, source, target);

  if ((flow == desired_flow) && (flow == n_outstanding_matches) && !flag) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int n_test_cases;
  std::cin >> n_test_cases;
	for (int test_case_index = 0; test_case_index < n_test_cases; test_case_index++) {
    testcase();
  }
	return 0;
}
