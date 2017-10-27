#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property>Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;

bool compare_edge(const pair<int,int> &a,const pair<int,int> &b) {
  return a.first < b. first or (a.first == b.first and a.second < b.second);
}

void test() {
  int n_nodes, n_edges, u, v;
  cin >> n_nodes >> n_edges;
  Graph graph(n_nodes);
  vector<pair<int, int> > edges(n_edges);
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    cin >> u >> v;
    edges[edge_index] = make_pair(u, v);
  }
  Edge edge;
  bool success;
  vector<pair<int, int> > disconnecting_edges;
  for (int cut_index = 0; cut_index < n_edges; cut_index++) {
    // Build cut graph.
    Graph graph = (n_nodes);
    for (int edge_index = 0; edge_index < n_edges; edge_index++) {
      if (edge_index != cut_index) {
        tie(edge, success) =
            add_edge(edges[edge_index].first, edges[edge_index].second, graph);
      }
    }
    // Compute number of components.
    vector<int> component_map(n_nodes);
    int first_node, second_node;
    if (connected_components(graph,
        make_iterator_property_map(component_map.begin(), get(vertex_index, graph))) > 1
        and n_nodes > 1) {
      first_node = min(edges[cut_index].first, edges[cut_index].second);
      second_node = max(edges[cut_index].first, edges[cut_index].second);
      disconnecting_edges.push_back(make_pair(first_node, second_node));
    }
  }

  sort(disconnecting_edges.begin(), disconnecting_edges.end(), compare_edge);
  cout << disconnecting_edges.size() << endl;
  for (vector<pair<int, int> >::const_iterator it = disconnecting_edges.begin();
      it != disconnecting_edges.end (); ++it) {
    cout << it->first << " " <<  it->second << endl;
}

}

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test();
  }
  return 0;
}
