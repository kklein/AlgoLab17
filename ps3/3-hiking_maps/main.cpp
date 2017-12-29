#include <algorithm>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>

//TODO(kevinkle): remove
#include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Line_2 L;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

void test_run() {
  int n_triangle_points = 6;
  int n_triangle_lines = 3;
  int n_points, n_map_parts;
  std::cin >> n_points >> n_map_parts;

  std::vector<S> legs(n_points - 1);

  int x1, x2, y1, y2;
  std::cin >> x1>> y1;
  for (int leg = 0; leg < n_points - 1; leg++) {
    std::cin >> x2 >> y2;
    legs[leg] = S(P(x1, y1), P(x2, y2));
    x1 = x2;
    y2 = y1;
  }

  int earliest_map_part = n_map_parts;
  int last_map_part = 0;
  bool map_part_taken = false;

  for (int map_part = 0; map_part < n_map_parts; map_part++) {
    std::vector<P> triangle_points(n_triangle_points);
    std::vector<L> triangle_lines(n_triangle_lines);
    int x, y;
    for (int point = 0; point < n_triangle_points; point++) {
      std::cin >> x >> y;
      triangle_points[point] = P(x ,y);
    }
    for (int line = 0; line < n_triangle_lines; line++) {
      triangle_lines[line] = L(triangle_points[2 * line], triangle_points[2 * line + 1]);
    }
    auto intersection1 = CGAL::intersection(triangle_lines[0], triangle_lines[1]);
    auto intersection2 =  CGAL::intersection(triangle_lines[1], triangle_lines[2]);
    auto intersection3 =  CGAL::intersection(triangle_lines[2], triangle_lines[0]);
    if (const P* intersection_point_1 = boost::get<P>(&*intersection1)) {
      if (const P* intersection_point_2 = boost::get<P>(&*intersection2)) {
        if (const P* intersection_point_3 = boost::get<P>(&*intersection3)) {
          T triangle = T(*intersection_point_1, *intersection_point_2, *intersection_point_3);
          for (int leg = 0; leg < n_points - 1; leg++) {
            if (CGAL::do_intersect(legs[leg], triangle)) {
              map_part_taken = true;
              earliest_map_part = std::min(earliest_map_part, map_part);
              last_map_part = std::max(last_map_part, map_part);
            }
          }
        }
      }
    }
   }
  if (map_part_taken) {
    std::cout << "first: " << earliest_map_part << " ,last: " << last_map_part << std::endl;
    std::cout << (last_map_part - earliest_map_part) << std::endl;
  }
  return;
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
