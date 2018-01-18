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
  int n_stations, n_satellites, n_links, station, satellite;
  int n_selected_stations = 0;
  int n_selected_satellites = 0;
  std::cin >> n_stations >> n_satellites >> n_links;

  Graph G(n_stations + n_satellites);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  for (int link_index = 0; link_index < n_links; link_index++) {
    std::cin >> station >> satellite;
    // eaG.addEdge(source, station, 1);
    eaG.addEdge(station, n_stations + satellite, 1);
    // eaG.addEdge(n_stations + satellite, target, 1);
  }
  for (int station_index = 0; station_index < n_stations; station_index++) {
    eaG.addEdge(source, station_index, 1);
  }
  for (int satellite_index = 0; satellite_index < n_satellites;
      satellite_index++) {
    eaG.addEdge(n_stations + satellite_index, target, 1);
  }

	// Model min vertex cover on bipartite graph by
  // i) finding a max flow in the shape of a maximum matching
  // ii) apply BFS in residual graph from source to target, mark nodes
  // ii) include all unmarked stations and all marked satellites in VC
	int flow = boost::push_relabel_max_flow(G, source, target);

	// BFS to find vertex set S
	std::vector<int> vis(n_stations + n_satellites + 2, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[source] = true; // Mark the source as visited
	Q.push(source);
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

  std::vector<int> selected_stations;
  std::vector<int> selected_satellites;
	// Output S
	for (int i = 0; i < n_stations + n_satellites; ++i) {
		if (vis[i] and i >= n_stations) {
      n_selected_satellites++;
      selected_satellites.push_back(i);
    } else if (!vis[i] and i < n_stations) {
      n_selected_stations++;
      selected_stations.push_back(i);
    }
	}
  std::cout << n_selected_stations << " " << n_selected_satellites << std::endl;
  for (int station_index = 0; station_index < n_selected_stations;
      station_index++) {
    std::cout << selected_stations[station_index] << " ";
  }
  for (int satellite_index = 0; satellite_index < n_selected_satellites;
      satellite_index++) {
    std::cout << selected_satellites[satellite_index] - n_stations << " ";
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
