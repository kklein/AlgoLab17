#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  int n_points;
  std::cin >> n_points;
  while (n_points > 0) {
    double x, y;
    std::vector<P> points(n_points);
    for (int point = 0; point < n_points; point++) {
      std::cin >> x >> y;
      points[point] = P(x, y);
    }
    Min_circle  mc(points.begin(), points.end(), true);
    CGAL::set_pretty_mode( std::cout);
    std::cout << ceil_to_double(CGAL::sqrt(mc.circle().squared_radius())) << std::endl;
    // std::cout << ceil_to_double(mc.circle().squared_radius()) << std::endl;
    std::cin >> n_points;
  }
  return 0;
}
