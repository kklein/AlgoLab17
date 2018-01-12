#include <algorithm>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <climits>
#include <iostream>
#include <vector>
// #include "prettyprint.hpp"

//TODO(kevinkle): remove
#include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Line_2 L;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

int pointers_to_distance(std::vector<int> &map_coverage_position,
    std::vector<std::vector<int> > &map_coverage) {
  std::vector<int> map_parts(map_coverage_position.size());
  for (int leg = 0; leg < map_coverage_position.size(); leg++) {
    map_parts[leg] = map_coverage[leg][map_coverage_position[leg]];
  }
  return 1 + *std::max_element(map_parts.begin(), map_parts.end()) - *std::min_element(map_parts.begin(), map_parts.end());
}

bool triangle_contains_segment(std::vector<P> &t, S &s) {
  P p1 = s.source();
  P p2 = s.target();
  return !CGAL::right_turn(t[0], t[1], p1) && !CGAL::right_turn(t[2], t[3], p1)
      && !CGAL::right_turn(t[4], t[5], p1) && !CGAL::right_turn(t[0], t[1], p2)
      && !CGAL::right_turn(t[2], t[3], p2) && !CGAL::right_turn(t[4], t[5], p2);
}

int move_to_next_distance(std::vector<int> &map_coverage_position,
    std::vector<std::vector<int> > &map_coverage) {
  int min_value = INT_MAX;
  int min_leg;
  for (int leg = 0; leg < map_coverage_position.size(); leg++) {
    if (map_coverage[leg][map_coverage_position[leg]] < min_value) {
      min_value = map_coverage[leg][map_coverage_position[leg]];
      min_leg = leg;
    }
  }
  if (map_coverage_position[min_leg] + 1 >= map_coverage[min_leg].size()) {
    return INT_MAX;
  } else {
    map_coverage_position[min_leg]++;
    return pointers_to_distance(map_coverage_position, map_coverage);
  }
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

  // Per leg, lists all maps covering it.
  std::vector<std::vector<int> > map_coverage(n_points - 1);

  for (int map_part = 0; map_part < n_map_parts; map_part++) {
    std::vector<P> triangle_points(n_triangle_points);
    int x, y;
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
        // std::cout << "triangle contains segment!" << std::endl;
        map_coverage[leg].push_back(map_part);
      }
    }
  }

  bool depleted = false;
  // Per leg, position in map_coverage.
  std::vector<int> map_coverage_position(n_points - 1, 0);

  for (int leg = 0; leg < n_points - 1; leg++) {
    std::sort(map_coverage[leg].begin(), map_coverage[leg].end());
  }
  int min_distance = pointers_to_distance(map_coverage_position, map_coverage);
  int next_distance;

  while (!depleted) {
    next_distance = move_to_next_distance(map_coverage_position, map_coverage);
    min_distance = std::min(min_distance, next_distance);
    if (next_distance == INT_MAX) {
      depleted = true;
    }
  }

  std::cout << min_distance << std::endl;
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
