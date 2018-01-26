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
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// #include "prettyprint.hpp"
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

void push_back_to_times_if_not_present(std::vector<int> &times, int t) {
  auto p = std::find(times.begin(), times.end(), t);
  if (p == times.end()) {
    times.push_back(t);
  }
}

int get_time_index(std::vector<int> &times, int t) {
  int result = std::distance(times.begin(),
      std::find(times.begin(), times.end(), t));
  return result;
}

void test_case() {
    int n_requests, n_stations;
    std::cin >> n_requests >> n_stations;

    int n_cars = 0;
    std::vector<int> car_availability(n_stations);
    for (int station_index = 0; station_index < n_stations; station_index++) {
      std::cin >> car_availability[station_index];
      n_cars += car_availability[station_index];
    }

    std::vector<int> times;
    times.push_back(0);
    std::vector<std::pair<int, int> > request_directions(n_requests);
    std::vector<std::pair<int, int> > request_times(n_requests);
    std::vector<int> profits(n_requests);
    for (int request_index = 0; request_index < n_requests; request_index++) {
      int station_a, station_b, departure_time, arrival_time;
      std::cin >> station_a >> station_b >> departure_time >> arrival_time >>
          profits[request_index];
      // Off-by-1 indexing.
      station_a--;
      station_b--;

      push_back_to_times_if_not_present(times, arrival_time);
      push_back_to_times_if_not_present(times, departure_time);

      request_directions[request_index] = std::make_pair(station_a, station_b);
      request_times[request_index] = std::make_pair(departure_time,
          arrival_time);
    }

    std::sort(times.begin(), times.end());

    std::vector<std::pair<int, int> > request_time_indeces(n_requests);
    for (int request_index = 0; request_index < n_requests; request_index++) {
      int index_arrival = get_time_index(times,
          request_times[request_index].first);
      int index_departure = get_time_index(times,
          request_times[request_index].second);
      request_time_indeces[request_index] =
          std::make_pair(index_arrival, index_departure);
    }

    Graph G(n_stations * (times.size()));
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);

    for (int station_index = 0; station_index < n_stations; station_index++) {
      eaG.addEdge(source, station_index * times.size() + 0,
          car_availability[station_index], 0);
      for (int time_index = 0; time_index < times.size() - 1; time_index++) {
        eaG.addEdge(station_index * times.size() + time_index,
            station_index * times.size() + time_index + 1,
            n_cars, 100);
      }
      eaG.addEdge(station_index * times.size() + times.size() - 1, target,
          n_cars, 0);
    }

    for (int request_index = 0; request_index < n_requests; request_index++) {
      std::pair<int, int> stations = request_directions[request_index];
      Vertex u = stations.first * times.size() +
          request_time_indeces[request_index].first;
      Vertex v = stations.second * times.size() +
          request_time_indeces[request_index].second;

      int time_step = request_time_indeces[request_index].second -
          request_time_indeces[request_index].first;
      eaG.addEdge(u, v, 1, time_step * 100 - profits[request_index]);
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    // // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }

    long actual_profit = (times.size() - 1) * 100 ;
    actual_profit = s_flow * actual_profit;
    actual_profit = actual_profit - cost2;
    std::cout << actual_profit << std::endl;
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
