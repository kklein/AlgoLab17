#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

bool compare_edge(const pair<int,int> &a,const pair<int,int> &b) {
  return a.first < b. first or (a.first == b.first and a.second < b.second);
}

void visit_node(Vertex vertex, Graph* graph, vector<int>* arrival_time, vector<int>* low,
    vector<int>* parent, int* counter, vector<pair<int, int> >* disconnecting_edges) {
  int current_value = (*counter)++;
  (*arrival_time)[vertex] = current_value;
  (*low)[vertex] = current_value;
  OutEdgeIt oebeg, oeend;
  for (tie(oebeg, oeend) = out_edges(vertex, *graph); oebeg != oeend; ++oebeg) {
   Vertex neighbor = target(*oebeg, *graph);
   if ((*arrival_time)[neighbor] == -1) {
     (*parent)[neighbor] = vertex;
     visit_node(neighbor, graph, arrival_time, low, parent, counter, disconnecting_edges);
     (*low)[vertex] = min((*low)[vertex], (*low)[neighbor]);
     if ((*low)[neighbor] > (*arrival_time)[vertex]) {
      int first_node = min(vertex, neighbor);
      int second_node = max(vertex, neighbor);
       (*disconnecting_edges).push_back(make_pair(first_node, second_node));
     }
   } else if ((*parent)[vertex] != neighbor) {
     (*low)[vertex] = min((*low)[vertex], (*arrival_time)[neighbor]);
   }
  }
}

void test() {
  int n_nodes, n_edges, u, v;
  cin >> n_nodes >> n_edges;
  if (n_nodes <= 0) {
    cout << 0 << endl;
    return;
  }
  Graph graph(n_nodes);
  vector<pair<int, int> > edges(n_edges);
  Edge edge;
  bool success;
  vector<pair<int, int> > disconnecting_edges;
  for (int edge_index = 0; edge_index < n_edges; edge_index++) {
    cin >> u >> v;
    edges[edge_index] = make_pair(u, v);
    tie(edge, success) = add_edge(u, v, graph);
  }

  Vertex vertex = 0;
  vector<int> arrival_time(n_nodes, -1);
  vector<int> low(n_nodes);
  vector<int> parent(n_nodes);
  int counter = 0;
  visit_node(vertex, &graph, &arrival_time, &low, &parent, &counter, &disconnecting_edges);

  sort(disconnecting_edges.begin(), disconnecting_edges.end(), compare_edge);
  cout << disconnecting_edges.size() << endl;
  for (vector<pair<int, int> >::const_iterator it = disconnecting_edges.begin();
      it != disconnecting_edges.end (); ++it) {
    cout << it->first << " " <<  it->second << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test();
  }
  return 0;
}
