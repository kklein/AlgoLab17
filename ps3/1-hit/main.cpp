#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

int main () {
  int n_segments;
  std::cin >> n_segments;
  while (n_segments > 0) {
    long long rx0;
    long long ry0;
    long long rx;
    long long ry;
    std::cin >> rx0;
    std::cin >> ry0;
    std::cin >> rx;
    std::cin >> ry;
    R ray = R(P(rx0, ry0), P(rx, ry));
    bool still_searching = true;
    for (int i = 0; i < n_segments; i++) {
      long long sx0;
      long long sy0;
      long long sx1;
      long long sy1;
      std::cin >> sx0;
      std::cin >> sy0;
      std::cin >> sx1;
      std::cin >> sy1;
      S segment = S(P(sx0, sy0), P(sx1, sy1));
      if (still_searching) {
        still_searching = not CGAL::do_intersect(ray, segment);
      }
    }
    std::cout << (still_searching ? "no" : "yes") << "\n";
    std::cin >> n_segments;
  }
  return 0;
}
