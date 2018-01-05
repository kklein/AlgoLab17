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

void testcases() {
  int width, n_bricks, coordinate_a, coordinate_b, start_coordinate,
      end_coordinate;
  std::cin >> width >> n_bricks;

  // Create Graph and Maps
  Graph G(2 * (width - 1));
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, revedgemap);

  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  // Introduce throttling edge per 'breakpoint' in order to ensure that each
  // breakpoint is used at most once.
  // Access the left node of the breakpoint i with 2 * (i - 1),the right node
  // with 2 * (i - 1) + 1.
  for (int width_index = 1; width_index < width; width_index++) {
    eaG.addEdge(2 * (width_index - 1), 2 * (width_index - 1) + 1, 1);
  }

  for (int brick_index = 0; brick_index < n_bricks; brick_index++) {
    std::cin >> coordinate_a >> coordinate_b;
    start_coordinate = std::min(coordinate_a, coordinate_b);
    end_coordinate = std::max(coordinate_a, coordinate_b);
    int left =
        (start_coordinate == 0) ? source : 2 * (start_coordinate - 1) + 1;
    int right = (end_coordinate == width) ? target : 2 * (end_coordinate - 1);
    eaG.addEdge(left, right, 1);
  }

  long flow = boost::push_relabel_max_flow(G, source, target);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_case_index = 0; test_case_index < n_test_cases; test_case_index++) {
    testcases();
  }
  return 0;
}
