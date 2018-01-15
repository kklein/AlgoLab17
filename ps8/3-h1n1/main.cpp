#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <queue>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK> Triangulation;
typedef CGAL::Point_2<IK> Point;
typedef CGAL::Segment_2<IK> Segment;

// In order to determine whether it is possible to reach an
// infinite face without ever getting 'too close' to a node,
// we do a BFS on the centers of edges of the Delaunay
// triangulation.
bool is_escapable(Point p, double squared_min_distance,
    Triangulation &t) {
  // Check initial state.
  if (CGAL::squared_distance(t.nearest_vertex(p)->point(), p) <
      squared_min_distance) {
    return false;
  } else if (t.is_infinite(t.locate(p))) {
    return true;
  }

  std::map<Triangulation::Face_handle, bool> visited;
  std::queue<Triangulation::Face_handle> q;
  bool escaped = false;
  Triangulation::Face_handle face_handle = t.locate(p);
  q.push(face_handle);
  visited[face_handle] = true;
  while (!escaped and !q.empty()) {
    face_handle = q.front();
    q.pop();
    for (int neighbour_edge_index = 0; neighbour_edge_index < 3;
        neighbour_edge_index++) {
      Triangulation::Segment edge =
          t.segment(face_handle, neighbour_edge_index);
      double squared_distance =
          CGAL::to_double(edge.squared_length());
          // Each vertex of the edge has to be fulfill the
          // the squared_min_distance constraint.
      if (squared_distance >= 4 * squared_min_distance) {
        Triangulation::Face_handle neighbour_face_handle =
            face_handle->neighbor(neighbour_edge_index);
        if (!visited[neighbour_face_handle]) {
          if (t.is_infinite(neighbour_face_handle)) {
            escaped = true;
            break;
          }
          q.push(neighbour_face_handle);
          visited[neighbour_face_handle] = true;
        }
      }
    }
  }
  return escaped;
}

void test_case(int n_infected) {
  std::vector<Point> infected(n_infected);
  for (int infected_index = 0; infected_index < n_infected;
      infected_index++) {
    std::cin >> infected[infected_index];
  }

  Triangulation t;
  t.insert(infected.begin(), infected.end());

  int n_queries;
  std::cin >> n_queries;
  Point query_position;
  double query_distance;
  for (int query_index = 0; query_index < n_queries;
      query_index++) {
    std::cin >> query_position;
    std::cin >> query_distance;
    if (is_escapable(query_position, query_distance, t)) {
      std::cout << "y";
    } else {
      std::cout << "n";
    }
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_infected;
  std::cin >> n_infected;
  while (n_infected > 0) {
    test_case(n_infected);
    std::cin >> n_infected;
  }
  return 0;
}
