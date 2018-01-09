#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <algorithm>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  // read number of points
  std::size_t n;
  std::cin >> n;
  while (n > 0) {
    // read points
    std::vector<K::Point_2> points;
    points.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
      K::Point_2 point;
      std::cin >> point;
      points.push_back(point);
    }
    // construct triangulation
    Triangulation t;
    t.insert(points.begin(), points.end());

    K::FT min_distance = CGAL::squared_distance(points[0], points[1]);

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end();
        ++e) {
      min_distance = std::min(min_distance, t.segment(e).squared_length());
    }

    min_distance = 50 * CGAL::sqrt(min_distance);
    std::cout << ceil_to_double(min_distance) << std::endl;
    std::cin >> n;
  }
}
