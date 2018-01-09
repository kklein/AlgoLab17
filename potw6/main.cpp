#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/number_utils.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef EK::Point_2 P;
typedef EK::Ray_2 R;
typedef EK::Segment_2 S;

void test_case() {
  int n_bikers;
  std::cin >> n_bikers;
  std::vector<double> origins(n_bikers);
  std::vector<std::vector<double> >
      destinations(n_bikers, std::vector<double>(2));
  std::vector<R> rays(n_bikers);
  std::vector<int> remaining_bikers;

  for (int biker_index = 0; biker_index < n_bikers; biker_index++) {
    std::cin >> origins[biker_index] >> destinations[biker_index][0] >>
        destinations[biker_index][1];
    P origin(0, origins[biker_index]);
    P destination(destinations[biker_index][0], destinations[biker_index][1]);
    rays[biker_index] = R(origin, destination);
  }

  
  for (int biker_a = 0; biker_a < n_bikers; biker_a++) {
    bool biker_alive = true;
    for (int biker_b = 0; biker_b < n_bikers; biker_b++) {
      if (biker_a != biker_b and
          CGAL::do_intersect(rays[biker_a], rays[biker_b])) {
        auto o = CGAL::intersection(rays[biker_a], rays[biker_b]);
        if (const P* op = boost::get<P>(&*o)) {
          EK::FT distance_a = CGAL::abs((op->y() - origins[biker_a]));
          EK::FT distance_b = CGAL::abs((op->y() - origins[biker_b]));
          // Apply right before left rule.
          if (distance_a < distance_b or (distance_a == distance_b and
              origins[biker_a] < origins[biker_b])) {
            // Do nothing.
          } else {
            biker_alive = false;
            break;
          }
        }
      }
    }
    if (biker_alive) {
      remaining_bikers.push_back(biker_a);
    }
  }


  for (int biker_index = 0; biker_index < remaining_bikers.size();
      biker_index++) {
    std::cout << remaining_bikers[biker_index] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
