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

    Edge addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
        return e;
    }
};

void test_case() {
  int n_nodes, n_edges, position_a, position_b, width, length;
  // An upper bound on the max flow by counting the outgoing capacity of
  // start node.
  int capacity_limit = 0;
  Vertex start, finish;

  std::cin >> n_nodes >> n_edges >> start >> finish;

  Graph G(n_nodes);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap =
      boost::get(boost::edge_residual_capacity, G);
  EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    std::cin >> position_a >> position_b >> width >> length;
    if (position_a == start or position_b == start) {
      capacity_limit += width;
    }
    eaG.addEdge(position_a, position_b, width, length);
    eaG.addEdge(position_b, position_a, width, length);
  }

  Vertex source = boost::add_vertex(G);

  // Determine shortest path by letting 1 runner flow trough graph.
  Edge src_edge = eaG.addEdge(source, start, 1, 0);
  boost::successive_shortest_path_nonnegative_weights(G, source, finish);
  int shortest_distance = boost::find_flow_cost(G);

  OutEdgeIt e, eend;
  long distance, mid_flow, flow;
  long low_flow = 1;
  long high_flow = capacity_limit + 1;

  // Apply binary search to find largest flow in which each flow unit
  // costs no more than the shortest path (i.e. minimal cost). Adapt the
  // flow by introducing an extra source node with 0 cost and adapted
  // capacity.
  while (low_flow != high_flow) {
    mid_flow = low_flow + (high_flow - low_flow) / 2;
    capacitymap[src_edge] = mid_flow;
    boost::successive_shortest_path_nonnegative_weights(G, source, finish);
    distance = boost::find_flow_cost(G);
    flow = 0;
    for (boost::tie(e, eend) =
        boost::out_edges(boost::vertex(source,G), G);e != eend; ++e) {
      flow += capacitymap[*e] - rescapacitymap[*e];
    }
    if (flow < mid_flow ||
        (distance + flow - 1) / flow > shortest_distance) {
      high_flow = mid_flow;
    }
    else {
      low_flow = mid_flow + 1;
    }
  }

  std::cout << low_flow - 1 << std::endl;
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
