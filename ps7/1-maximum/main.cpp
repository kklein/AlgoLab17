#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

int main() {
  int problem_type;
  std::cin >> problem_type;
  while (problem_type != 0) {
    int a,b;
    std::cin >> a >> b;
    Program qp;
    if (problem_type == 1) {
      qp = Program(CGAL::SMALLER, true, 0, false, 0);
      qp.set_a(0, 0, 1);
      qp.set_a(1, 0, 1);
      qp.set_b(0, 4);
      qp.set_a(0, 1, 4);
      qp.set_a(1, 1, 2);
      qp.set_b(1, a*b);
      qp.set_a(0, 2, -1);
      qp.set_a(1, 2, 1);
      qp.set_b(2, 1);
      qp.set_d(0, 0, 2*a);
      qp.set_c(1, -b);

      Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
      assert(s.solves_quadratic_program(qp));
      if (s.is_unbounded()) {
        std::cout << "unbounded" << std::endl;
      } else if (s.is_infeasible()) {
        std::cout << "no" << std::endl;
      } else {
        std::cout << floor_to_double(-s.objective_value()) << std::endl;
      }
    } else {
      qp = Program(CGAL::LARGER, false, 0, false, 0);
      qp.set_u(0, true, 0);
      qp.set_u(1, true, 0);
      qp.set_l(2, true, 0);
      qp.set_a(0, 0, 1);
      qp.set_a(1, 0, 1);
      qp.set_a(2, 0, 0);
      qp.set_a(0, 1, 4);
      qp.set_a(1, 1, 2);
      qp.set_a(2, 1, 1);
      qp.set_a(0, 2, -1);
      qp.set_a(1, 2, 1);
      qp.set_a(2, 2, 0);
      qp.set_b(0, -4);
      qp.set_b(1, -a*b);
      qp.set_b(2, -1);
      qp.set_c(1, b);
      qp.set_d(0, 0, 2*a);
      qp.set_d(2, 2, 2*1);

      Solution s = CGAL::solve_quadratic_program(qp, ET());
      assert(s.solves_quadratic_program(qp));
      if (s.is_unbounded()) {
        std::cout << "unbounded" << std::endl;
      } else if (s.is_infeasible()) {
        std::cout << "no" << std::endl;
      } else {
        std::cout << ceil_to_double(s.objective_value()) << std::endl;
      }
    }

    std::cin >> problem_type;
  }
  return 0;
}
