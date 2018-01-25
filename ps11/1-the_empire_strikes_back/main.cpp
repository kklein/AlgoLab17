#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/basic.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// TODO(kevinlke): test integral type
typedef CGAL::Gmpq ET;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void test_case() {
  int n_particles, n_shootings, n_hunters, energy_limit, density;
  Point point;
  std:: cin >> n_particles >> n_shootings >> n_hunters >> energy_limit;

  Program lp (CGAL::LARGER, true, 0, false, 0);

  std::vector<Point> particle_position(n_particles);
  for (int particle_index = 0; particle_index < n_particles; particle_index++) {
    std::cin >> particle_position[particle_index] >> density;
    lp.set_b(particle_index, density);
  }

  std::vector<Point> shooting_position(n_shootings);
  for (int shooting_index = 0; shooting_index < n_shootings; shooting_index++) {
    std::cin >> shooting_position[shooting_index];
    lp.set_c(shooting_index, 1);
  }

  std::vector<Point> hunter_position(n_hunters);
  for (int hunter_index = 0; hunter_index < n_hunters; hunter_index++) {
    std::cin >> hunter_position[hunter_index];
  }

  Triangulation t;
  t.insert(hunter_position.begin(), hunter_position.end());

  for (int shooting_index = 0; shooting_index < n_shootings; shooting_index++) {
    Point shooting_point = shooting_position[shooting_index];
    K::FT max_squared_radius = -1;
    if (n_hunters > 0) {
      Point nearest_hunter = t.nearest_vertex(shooting_point) -> point();
      max_squared_radius = CGAL::squared_distance(shooting_point,
          nearest_hunter);
    }
    for (int particle_index = 0; particle_index < n_particles; particle_index++) {
      K::FT squared_distance = CGAL::squared_distance(shooting_point,
          particle_position[particle_index]);
      if (max_squared_radius == -1 or squared_distance < max_squared_radius) {
        if (squared_distance > 0) {
          lp.set_a(shooting_index, particle_index, 1 / squared_distance);
        } else {
          lp.set_a(shooting_index, particle_index, 1);
        }
      // } else {
      //   lp.set_a(shooting_index, particle_index, 0);
      }
    }
  }

  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible() or s.objective_value() > energy_limit) {
    std::cout << 'n' << std::endl;
  } else {
    std::cout << 'y' << std::endl;
  }

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
