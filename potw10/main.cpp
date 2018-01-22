// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void test_case() {
  int n_intersections, n_streets, n_police, n_photos;
  std::cin >> n_intersections >> n_streets >> n_police >> n_photos;

  // Create a part of that graph that can be used by policemen not carrying
  // photos and a part of the graph than can be used policemen carrying
  // photos.
	Graph G(2 * n_intersections);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);

  for (int police_index = 0; police_index < n_police; police_index++) {
    int intersection;
    std::cin >> intersection;
    // Exactly 1 policeman per police station.
    eaG.addEdge(source, intersection, 1);
    // Exactly 1 safe per police station.
    eaG.addEdge(n_intersections + intersection, target, 1);
  }

  for (int photo_index = 0; photo_index < n_photos; photo_index++) {
    int intersection;
    std::cin >> intersection;
    // A photo can be taken at most once and once a photo is taken by a
    // policeman, he can't pick up another one as the first 'half' of the
    // graph becomes unreachable.
    eaG.addEdge(intersection, n_intersections + intersection, 1);
  }

  for (int street_index = 0; street_index < n_streets; street_index++) {
    int intersection_a, intersection_b;
    std::cin >> intersection_a >> intersection_b;
    // Each edge can be used an arbitrary number of times by policemen not
    // carrying a photo.
    eaG.addEdge(intersection_a, intersection_b, n_police);
    // Each edge can be used at most once by policemen carrying a photo.
    eaG.addEdge(n_intersections + intersection_a,
        n_intersections + intersection_b, 1);
  }

	long flow = boost::push_relabel_max_flow(G, source, target);
  std::cout << flow << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
	return 0;
}
