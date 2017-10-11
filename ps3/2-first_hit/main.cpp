#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <limits>

//TODO(kevinkle): remove
#include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void test_case(int n_segments) {
  long long rx0, ry0, rx, ry;
  std::cin >> rx0 >> ry0 >> rx >> ry;
  P p_source(rx0, ry0);
  P p_ray(rx, ry);
  R ray(p_source, p_ray);
  K::FT min_distance;
  P min_intersection;
  bool found = false;

  for (int i = 0; i < n_segments; i++) {
    long long sx0, sy0, sx1, sy1;
    std::cin >> sx0 >> sy0 >> sx1 >> sy1;
    P p0(sx0, sy0);
    P p1(sx1, sy1);
    S segment(p0, p1);
    if (not CGAL::do_intersect(ray, segment)) {
      continue;
    };
    auto intersection = CGAL::intersection(ray, segment);
    K::FT squared_distance;
    P intersection_point;
    if (const P* point = boost::get<P>(&*intersection)) {
      intersection_point = *point;
    } else if (const S* intersection_segment = boost::get<S>(&*intersection)) {
      if (CGAL::squared_distance(p_source, intersection_segment->source())
          < CGAL::squared_distance(p_source, intersection_segment->target())) {
        intersection_point = intersection_segment->source();
      } else {
        intersection_point = intersection_segment->target();
      }
    } else {
      std::cout << "something's super messed up" << std::endl;
    }
    squared_distance = CGAL::squared_distance(p_source, intersection_point);
    if (not found or (squared_distance < min_distance)) {
      min_distance = squared_distance;
      min_intersection = intersection_point;
      found = true;
    }
  }

  if (found) {
    std::cout << std::fixed;
    std::cout << floor_to_double(min_intersection.x()) << ' ' <<
        floor_to_double(min_intersection.y()) << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}

int main () {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int n_segments;
  std::cin >> n_segments;
  while (n_segments > 0) {
    test_case(n_segments);
    std::cin >> n_segments;
  }
  return 0;
}
