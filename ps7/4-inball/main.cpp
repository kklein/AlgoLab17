#include <cmath>
#include <iostream>
#include <vector>

#include <CGAL/basic.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_inequalities, n_dimensions, coefficient;
  std::cin >> n_inequalities;
  while (n_inequalities > 0) {
    Program lp (CGAL::SMALLER, false, 0, false, 0);
    std::cin >> n_dimensions;
    for (int inequality_index = 0; inequality_index < n_inequalities; inequality_index++) {
      double norm = 0;
      for (int dimension_index = 0; dimension_index < n_dimensions; dimension_index++) {
        std::cin >> coefficient;
        norm += coefficient * coefficient;
        lp.set_a(dimension_index, inequality_index, coefficient);
      }
      norm = std::sqrt(norm);
      lp.set_a(n_dimensions, inequality_index, norm);
      std::cin >> coefficient;
      lp.set_b(inequality_index, coefficient);
    }
    lp.set_c(n_dimensions, -1);
    lp.set_l(n_dimensions, true, 0);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()) {
      std::cout << - s.objective_value().numerator() / s.objective_value().denominator() << std::endl;
    } else if (s.is_unbounded()){
      std::cout << "inf" << std::endl;
    } else {
      std::cout << "none" << std::endl;
    }
    std::cin >> n_inequalities;
  }
  return 0;
}
