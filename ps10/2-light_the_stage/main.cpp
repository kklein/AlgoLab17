#include <algorithm>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;

// Considering the lamps l_0 up to and exlcuding l_{0 + offset}.
void determine_winners(std::vector<Point> &lamps,
    std::vector<std::pair<Point, K::FT> > &participants, K::FT height,
    int offset, std::vector<int> &winners) {
  winners.clear();
  Triangulation t;
  t.insert(lamps.begin(), lamps.begin() + offset);
  for (int participant_index = 0; participant_index < participants.size();
      participant_index++) {
    Point participant_center = participants[participant_index].first;
    K::FT participant_radius = participants[participant_index].second;
    K::FT reference =
        (height + participant_radius) * (height + participant_radius);
    Point light_center = t.nearest_vertex(participant_center) -> point();
    if (CGAL::squared_distance(participant_center, light_center) >= reference) {
      winners.push_back(participant_index);
    }
  }
}

void test_case() {
  int n_participants, n_lamps;
  K::FT height;
  std::cin >> n_participants >> n_lamps;
  std::vector<std::pair<Point, K::FT> > participants(n_participants);
  std::vector<Point> lamps(n_lamps);
  std::vector<int> winners;
  for (int participant_index = 0; participant_index < n_participants;
      participant_index++) {
    Point p;
    K::FT r;
    std::cin >> p >> r;
    participants[participant_index] = std::make_pair(p, r);
  }
  std::cin >> height;
  for (int lamp_index = 0; lamp_index < n_lamps; lamp_index++) {
    std::cin >> lamps[lamp_index];
  }
  determine_winners(lamps, participants, height, n_lamps, winners);

  // Case in which some participants survive all lights.
  if (winners.size() > 0) {
    for (int winner: winners) {
      std::cout << winner << " ";
    }
    std::cout << std::endl;
    return;
  }

  // Case in which no participant survives all lights and the winners are the
  // ones remaining longest. Determine smallest offset with which no
  // participant survives. Then determine winners by applying the algorithm for
  // offset - 1.
  int l_min = 1;
  int l_max = n_lamps;
  while (l_min != l_max) {
    int p = (l_min + l_max) / 2;
    determine_winners(lamps, participants, height, p, winners);
    if (winners.size() > 0) {
      l_min = p + 1;
    } else {
      l_max = p;
    }
  }

  int l = l_min - 1;
  determine_winners(lamps, participants, height, l, winners);
  for (int winner: winners) {
    std::cout << winner << " ";
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
}
