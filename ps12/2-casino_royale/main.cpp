#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void test_case() {
  int n_stops, n_missions, capacity;
  std::cin >> n_stops >> n_missions >> capacity;
  Program lp (CGAL::SMALLER, true, 0, true, 1);

  for (int stop = 0; stop < n_stops - 1; stop++) {
    lp.set_b(stop, capacity);
  }

  for (int mission_index = 0; mission_index < n_missions; mission_index++) {
    int stop_a, stop_b, priority;
    std::cin >> stop_a >> stop_b >> priority;
    lp.set_c(mission_index, -priority);
    for (int stop = stop_a; stop < stop_b; stop++) {
      lp.set_a(mission_index, stop, 1);
    }
  }

  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));

  std::cout << -s.objective_value() << std::endl;

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
