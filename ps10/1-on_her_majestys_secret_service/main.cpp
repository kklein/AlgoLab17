#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Include edges from agents to shelters if they allow for an entry into the
// shelter within the given time constraints. Apply a bipartite matching on this
// the resulting graph and determine whether all agents were able to be
// sheltered.
bool large_enough(int t, std::vector<std::vector<int> > &distances,
    int n_agents, int n_shelters, int additional_cost, int shelter_capacity) {
  Graph G (n_agents + shelter_capacity * n_shelters);
  for (int agent_index = 0; agent_index < n_agents; agent_index++) {
    for (int shelter_index = 0; shelter_index < n_shelters; shelter_index++) {
      for (int capacity = 0; capacity < shelter_capacity; capacity++) {
        if (distances[agent_index][shelter_index] < INT_MAX and
            distances[agent_index][shelter_index] + (capacity + 1) *
            additional_cost <= t) {
          boost::add_edge(agent_index, n_agents + capacity * n_shelters +
              shelter_index, G);
        }
      }
    }
  }

	std::vector<Vertex> matemap(n_agents + shelter_capacity * n_shelters);
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));

  // Alternative matching size computation:
	// int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
  const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
  int matchingsize = 0;
  for (int agent_index = 0; agent_index < n_agents; agent_index++) {
    if (matemap[agent_index] != NULL_VERTEX) {
      matchingsize++;
    }
  }

  return matchingsize == n_agents;
}

void test_case() {
  int n_locations, n_edges, n_agents, n_shelters, shelter_capacity,
      shelter_delay;
  std::cin >> n_locations >> n_edges >> n_agents >> n_shelters >>
      shelter_capacity >> shelter_delay;

  Graph G(n_locations);
  WeightMap weightmap = boost::get(boost::edge_weight, G);

  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    char type;
    int location_a, location_b, cost;
    Edge e;
    bool success;
    std::cin >> type >> location_a >> location_b >> cost;
    boost::tie(e, success) = boost::add_edge(location_a, location_b, G);
    weightmap[e] = cost;
    if (type == 'L') {
      boost::tie(e, success) = boost::add_edge(location_b, location_a, G);
      weightmap[e] = cost;
    }
  }

  std::vector<int> agent_location(n_agents);
  for (int agent_index = 0; agent_index < n_agents; agent_index++) {
    std::cin >> agent_location[agent_index];
  }

  std::vector<int> shelter_location(n_shelters);
  for (int shelter_index = 0; shelter_index < n_shelters; shelter_index++) {
    std::cin >> shelter_location[shelter_index];
  }

  std::vector<std::vector<int> > distances(n_agents,
      std::vector<int>(n_shelters, INT_MAX));
  for (int agent_index = 0; agent_index < n_agents; agent_index++) {
  	std::vector<int> distmap(n_locations);
  	dijkstra_shortest_paths(G, agent_location[agent_index],
  		distance_map(boost::make_iterator_property_map(distmap.begin(), get(boost::vertex_index, G))));

    for (int shelter_index = 0; shelter_index < n_shelters; shelter_index++) {
      distances[agent_index][shelter_index] =
          distmap[shelter_location[shelter_index]];
    }
  }

  // Binary search to find minimal time that allows every agent to arrive in
  // a shelter.
  int tmin = 0;
  int tmax = INT_MAX;

  while (tmin < tmax) {
    int t = tmin + (tmax - tmin) / 2;
    if (large_enough(t, distances, n_agents, n_shelters,
        shelter_delay, shelter_capacity)) {
      tmax = t;
    } else {
      tmin = t + 1;
    }
  }

  std::cout << tmin << std::endl;
}

int main() {
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
