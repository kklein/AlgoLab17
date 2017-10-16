#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <vector>

//TODO(kevinkle): remove
#include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Line_2 L;
typedef K::Point_2 Point;
typedef K::Segment_2 S;
typedef K::Triangle_2 T;

void test_run() {
  int n_triangle_points = 6;
  int n_triangle_lines = 3;
  int n_points, n_map_parts;
  std::cin >> n_points >> n_map_parts;

  std::vector<Point> points(n_points);

  int x, y;
  for (int point = 0; point < n_points; point++) {
    std::cin >> x >> y;
    points[point] = Point(x, y);
  }

  Polygon polygon = Polygon(points.begin(), points.end());
  std::cout << polygon << std::endl;

  for (int map_part = 0; map_part < n_map_parts; map_part++) {
    std::vector<Point> triangle_points(n_triangle_points);
    std::vector<L> triangle_lines(n_triangle_lines);
    int x,y;
    for (int point = 0; point < n_triangle_points; point++) {
      std::cin >> x >> y;
      triangle_points[point] = Point(x ,y);
    }
    for (int line = 0; line < n_triangle_lines; line++) {
      triangle_lines[line] = L(triangle_points[2 * line], triangle_points[2 * line + 1]);
    }
    auto intersection1 = CGAL::intersection(triangle_lines[0], triangle_lines[1]);
    auto intersection2 =  CGAL::intersection(triangle_lines[1], triangle_lines[2]);
    auto intersection3 =  CGAL::intersection(triangle_lines[2], triangle_lines[0]);
    std::vector<Point> intersection_points(n_triangle_lines);
    if (intersection_points[0] = boost::get<Point>(&*intersection1) and
        intersection_points[1] = boost::get<P>(&*intersection2) and
        intersection_points[2] = boost::get<P>(&*intersection3)) {
      Polygon triangle = Polygon(intersection_points.begin(), intersection_points.end());
      auto intersection = CGAL::intersection(polygon, triangle);
      std::cout << "polygon intersection: " <<  intersection << std::endl;
     }
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
