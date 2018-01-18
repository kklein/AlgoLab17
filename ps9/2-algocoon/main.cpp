// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void test_case() {
  // std::cout << "starting test case" << std::endl;
  int n_figures, n_limbs, figure_a, figure_b, cost;
  std::cin >> n_figures >> n_limbs;
  // std::cout << "read input" << std::endl;
  Graph G(n_figures);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
  for (int limb_index = 0; limb_index < n_limbs; limb_index++) {
    std::cin >> figure_a >> figure_b >> cost;
    eaG.addEdge(figure_a, figure_b, cost);
  }

  int best_flow = INT_MAX;
  Vertex best_source = 0;
  Vertex best_target = n_figures - 1;

  // Instead of iterating over all n(n-1)/2 pairs, it suffices to investigate
  // flows which are sourced in 0 or target 0 as either of both parties has to
  // include 0.
  int source = 0;
  int target;
  for (target = 1; target < n_figures; target++) {
    int flow = boost::push_relabel_max_flow(G, source, target);
    if (flow < best_flow) {
      best_flow = flow;
      best_source = source;
      best_target = target;
    }
  }
  target = 0;
  for (source = 1; source < n_figures; source++) {
    int flow = boost::push_relabel_max_flow(G, source, target);
    if (flow < best_flow) {
      best_flow = flow;
      best_source = source;
      best_target = target;
    }
  }

	int flow = boost::push_relabel_max_flow(G, best_source, best_target);
  std::cout << flow << std::endl;

  // BFS to find vertex set S
	std::vector<int> vis(n_figures, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[best_source] = true; // Mark the source as visited
	Q.push(best_source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

  int n_selected_figures = 0;
  std::vector<int> selected_figures;
	// Output S
	for (int i = 0; i < n_figures; ++i) {
		if (vis[i]) {
      n_selected_figures++;
      selected_figures.push_back(i);
    }
	}
  std::cout << n_selected_figures << " ";
  for (int figure_index = 0; figure_index < n_selected_figures; figure_index++)
      {
    std::cout << selected_figures[figure_index] << " ";
  }
	std::cout << std::endl;
}

int main() {
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
