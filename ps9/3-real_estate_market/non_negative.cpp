// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void test_case() {
  int n_buyers, n_sites, n_states, price;
  std::cin >> n_buyers >> n_sites >> n_states;
  std::vector<int> limits(n_states);
  std::vector<int> state_affiliation(n_sites);

  // Create Graph and Maps
  Graph G(n_states + n_buyers + n_sites);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  for (int state_index = 0; state_index < n_states; state_index++) {
    std::cin >> limits[state_index];
    eaG.addEdge(source, state_index, limits[state_index], 0);
  }
  for (int site_index = 0; site_index < n_sites; site_index++) {
    std::cin >> state_affiliation[site_index];
    state_affiliation[site_index]--;
    eaG.addEdge(state_affiliation[site_index], n_states + site_index, 1, 0);
  }
  for (int buyer_index = 0; buyer_index < n_buyers; buyer_index++) {
    eaG.addEdge(n_states + n_sites + buyer_index, target, 1, 0);
    for (int site_index = 0; site_index < n_sites; site_index++) {
      std::cin >> price;
      // We know that each price is less than 100. Hence we can add 100 and turn
      // the cost into a positive number while still minimizing.
      eaG.addEdge(n_states + site_index, n_states + n_sites + buyer_index, 1,
          100 - price);
    }
  }

  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  int cost = boost::find_flow_cost(G);
  int flow = 0;
  // Iterate over all edges leaving the source to sum up the flow values.
  OutEdgeIt e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G);
      e != eend; ++e) {
    flow += capacitymap[*e] - rescapacitymap[*e];
  }
  std::cout << flow << " " << flow * 100 - cost << std::endl;
}

int main() {
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
}
