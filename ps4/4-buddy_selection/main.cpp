#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIt;

void test() {
  int n_students, n_characteristics, threshold;
  string characteristic;
  cin >> n_students >> n_characteristics >> threshold;
  vector<vector<string> > characteristics(n_students, vector<string>(n_characteristics));
  Graph graph(n_students);
  Edge edge;
  bool success;

  for (int student_index = 0; student_index < n_students; student_index++) {
    for (int characteristic_index = 0; characteristic_index < n_characteristics;
        characteristic_index++) {
      cin >> characteristic;
      characteristics[student_index][characteristic_index] = characteristic;
    }
    sort(characteristics[student_index].begin(), characteristics[student_index].end());
    for (int previous_student = 0; previous_student < student_index; previous_student++) {
      set<string> intersection;
      set_intersection(characteristics[student_index].begin(),
          characteristics[student_index].end(), characteristics[previous_student].begin(),
          characteristics[previous_student].end(), inserter(intersection, intersection.begin()));
      if (intersection.size() > threshold) {
        tie(edge, success) = add_edge(previous_student, student_index, graph);
      }
    }
  }

  vector<Vertex> matemap(n_students);
  edmonds_maximum_cardinality_matching(graph,
      make_iterator_property_map(matemap.begin(), get(vertex_index, graph)));
  int matchingsize = matching_size(graph, make_iterator_property_map(matemap.begin(),
      get(vertex_index, graph)));
  string result = 2 * matchingsize ==  n_students ? "not optimal" : "optimal";
  cout << result << endl;
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
