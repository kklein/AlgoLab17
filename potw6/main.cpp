#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/number_utils.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

struct intersection {
  K::FT x;
  K::FT y;
  int biker_a;
  int biker_b;
  K::FT min_distance;
  int last_biker;
};

bool compare_by_time(intersection &intersection_a,
    intersection &intersection_b) {
  return (intersection_a.x < intersection_b.x or
      (intersection_a.x == intersection_b.x and
      intersection_a.min_distance < intersection_b.min_distance));
}

void test_case() {
  int n_bikers;
  std::cin >> n_bikers;
  std::vector<double> origins(n_bikers);
  std::vector<std::vector<double> >
      destinations(n_bikers, std::vector<double>(2));
  std::vector<R> rays(n_bikers);
  std::vector<intersection> intersections;

  for (int biker_index = 0; biker_index < n_bikers; biker_index++) {
    std::cin >> origins[biker_index] >> destinations[biker_index][0] >>
        destinations[biker_index][1];
    P origin(0, origins[biker_index]);
    P destination(destinations[biker_index][0], destinations[biker_index][1]);
    rays[biker_index] = R(origin, destination);
  }

  // Detect all intersections by iterating over all biker pairs.
  for (int biker_a = 0; biker_a < n_bikers; biker_a++) {
    for (int biker_b = biker_a + 1; biker_b < n_bikers; biker_b++) {
      if (CGAL::do_intersect(rays[biker_a], rays[biker_b])) {
        auto o = CGAL::intersection(rays[biker_a], rays[biker_b]);
        if (const P* op = boost::get<P>(&*o)) {
          K::FT distance_a = CGAL::abs((op->y() - origins[biker_a]));
          K::FT distance_b = CGAL::abs((op->y() - origins[biker_b]));
          int last_biker;
          K::FT min_distance;
          // Apply right before left rule.
          if (distance_a < distance_b or (distance_a == distance_b and
              origins[biker_a] < origins[biker_b])) {
            last_biker = biker_b;
            min_distance = distance_a;
          } else {
            last_biker = biker_a;
            min_distance = distance_b;
          }
          intersection i =
              {op->x(), op->y(), biker_a, biker_b, min_distance, last_biker};
          intersections.push_back(i);
        }
      }
    }
  }

  // Sort intersections according to the time of the intersection.
  std::sort(intersections.begin(), intersections.end(), compare_by_time);

  // for (int intersection_index = 0; intersection_index < intersections.size();
  //     intersection_index++) {
  //   intersection i = intersections[intersection_index];
    // std::cout << "intersection between " << i.biker_a << " and " << i.biker_b <<
        // " at x=" << i.x << " y= " << i.y << std::endl;
  // }

  std::vector<bool> biker_alive(n_bikers, true);

  for (int intersection_index = 0; intersection_index < intersections.size();
      intersection_index++) {
    intersection i = intersections[intersection_index];
    // If either biker has already stopped, the intersection is irrelevant.
    if (biker_alive[i.biker_a] and biker_alive[i.biker_b]) {
      // std::cout << "Both bikers alive at ";
      // std::cout << "intersection between " << i.biker_a << " and " << i.biker_b << std::endl;
      biker_alive[i.last_biker] = false;
      // std::cout << "Biker " << i.last_biker << " stops." << std::endl;
    }
  }

  for (int biker_index = 0; biker_index < n_bikers; biker_index++) {
    if (biker_alive[biker_index]) {
      std::cout << biker_index << " ";
    }
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
