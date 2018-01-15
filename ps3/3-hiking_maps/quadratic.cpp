#include <algorithm>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <climits>
#include <iostream>
#include <vector>
// #include "prettyprint.hpp"

//TODO(kevinkle): remove
// #include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Line_2 L;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

bool triangle_contains_segment(std::vector<P> &t, S &s) {
  P p1 = s.source();
  P p2 = s.target();
  return !CGAL::right_turn(t[0], t[1], p1) && !CGAL::right_turn(t[2], t[3], p1)
      && !CGAL::right_turn(t[4], t[5], p1) && !CGAL::right_turn(t[0], t[1], p2)
      && !CGAL::right_turn(t[2], t[3], p2) && !CGAL::right_turn(t[4], t[5], p2);
}

void test_run() {
  int n_triangle_points = 6;
  int n_triangle_lines = 3;
  int n_points, n_map_parts;
  std::cin >> n_points >> n_map_parts;

  std::vector<S> leg_segments(n_points - 1);

  int x1, x2, y1, y2;
  std::cin >> x1 >> y1;
  for (int leg = 0; leg < n_points - 1; leg++) {
    std::cin >> x2 >> y2;
    leg_segments[leg] = S(P(x1, y1), P(x2, y2));
    x1 = x2;
    y1 = y2;
  }

  // Per map, list all legs covered.
  std::vector<std::vector<int> > leg_coverage(n_map_parts);

  for (int map_part = 0; map_part < n_map_parts; map_part++) {
    std::vector<P> triangle_points(n_triangle_points);
    for (int point = 0; point < n_triangle_points; point++) {
      std::cin >> triangle_points[point];
    }
    // Ensure correct order for orientation tests.
    for (int j = 0; j < n_triangle_points; j += 2){
      if (CGAL::right_turn(triangle_points[j], triangle_points[j + 1],
          triangle_points[(j + 2) % 6])){
        std::swap(triangle_points[j],triangle_points[j + 1]);
      }
    }

    for (int leg = 0; leg < n_points - 1; leg++) {
      if (triangle_contains_segment(triangle_points, leg_segments[leg])) {
        leg_coverage[map_part].push_back(leg);
      }
    }
  }

  // All map parts from [left_map_pointer, right_map_pointer[ are currently
  // included.
  int left_map_pointer = 0;
  int right_map_pointer = 0;
  // Counts the number maps covering a leg in current selection.
  std::vector<int> covered(n_points - 1, 0);
  // Counts the number of uncovered legs in current selection.
  int n_uncovered = n_points - 1;
  bool done = false;
  int shortest_range = INT_MAX;

  while (!done and left_map_pointer < n_map_parts) {
    if (n_uncovered == 0) {
      shortest_range = std::min(shortest_range,
          right_map_pointer - left_map_pointer);
      for (int leg: leg_coverage[left_map_pointer]) {
        covered[leg]--;
        if (covered[leg] == 0) {
          n_uncovered++;
        }
      }
      left_map_pointer++;

    } else if (right_map_pointer < n_map_parts) {
      for (int leg: leg_coverage[right_map_pointer]) {
        covered[leg]++;
        if (covered[leg] == 1) {
          n_uncovered--;
        }
      }
      right_map_pointer++;
    } else {
      done = true;
    }
  }

  std::cout << shortest_range << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    test_run();
  }
  return 0;
}
