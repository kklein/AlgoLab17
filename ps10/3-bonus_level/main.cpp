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
#include <climits>
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

// The key insight for this problem is that instead of explicitly modeling a
// passage from the top left to the bottom right corner and back again, we can
// can implicitly model this by having 2 passages from the top left to the
// bottom right. This allows us to control the unique usage of every available
// coin by applying a vertex capacity trick.

void test_case() {
  int dimension;
  std::cin >> dimension;

  // Create Graph and Maps
  Graph G(2 * (dimension * dimension));
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  eaG.addEdge(source, 0, 2, 0);
  eaG.addEdge(2 * dimension * dimension - 1, target, 2, 0);

  for (int i = 0; i < dimension; i++) {
    for (int j = 0; j < dimension; j++) {
      int coin_value;
      std::cin >> coin_value;
      eaG.addEdge(2 * (i * dimension + j), 2 * (i * dimension + j) + 1, 1,
          100 - coin_value);
      eaG.addEdge(2 * (i * dimension + j), 2 * (i * dimension + j) + 1, 1,
          100);
      if (i < dimension - 1) {
        // Add edge to 'down'.
        eaG.addEdge(2 * (i * dimension + j) + 1, 2 * ((i + 1) * dimension + j),
            2, 0);
      }
      if (j < dimension - 1) {
        // Add edge to 'right'.
        eaG.addEdge(2 * (i * dimension + j) + 1, 2 * (i * dimension + j + 1),
            2, 0);
      }
    }
  }
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  long cost = boost::find_flow_cost(G);
  // There are 2 flow units visiting 2 * dimension - 1 cells on their path
  // through the grid.
  std::cout << 2 * (2 * dimension - 1) * 100 - cost << std::endl;

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin>> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
}
