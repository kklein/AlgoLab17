#include <algorithm>
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Quotient<ET> SolT;
typedef K::Point_2 Point;

// round up to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void test_case() {
  int n_warehouses, n_stadiums, n_lines;
  std::cin >> n_warehouses >> n_stadiums >> n_lines;

  std::vector<Point> warehouse_location(n_warehouses),
      stadium_position(n_stadiums), line_center(n_lines);
  std::vector<long> warehouse_supply(n_warehouses), stadium_demand(n_stadiums),
      stadium_alcohol(n_stadiums), line_radius(n_lines);
  std::vector<double> warehouse_alcohol(n_warehouses);
  std::vector<std::vector<double> > revenues(n_warehouses,
      std::vector<double>(n_stadiums));
  long max_supply = 0;
  for (int warehouse_index = 0; warehouse_index < n_warehouses;
      warehouse_index++) {
    std::cin >> warehouse_location[warehouse_index] >>
        warehouse_supply[warehouse_index] >> warehouse_alcohol[warehouse_index];
    max_supply = std::max(max_supply, warehouse_supply[warehouse_index]);
  }
  for (int stadium_index = 0; stadium_index < n_stadiums; stadium_index++) {
    std::cin >> stadium_position[stadium_index] >> stadium_demand[stadium_index]
        >> stadium_alcohol[stadium_index];
  }
  for (int warehouse_index = 0; warehouse_index < n_warehouses;
      warehouse_index++) {
    for (int stadium_index = 0; stadium_index < n_stadiums;
        stadium_index++) {
      int revenue;
      std::cin >> revenues[warehouse_index][stadium_index];
    }
  }
  for (int line_index = 0; line_index < n_lines; line_index++) {
    std::cin >> line_center[line_index] >> line_radius[line_index];
  }

  Program lp (CGAL::SMALLER, true, 0, true, max_supply);

  for (int stadium_index = 0; stadium_index < n_stadiums; stadium_index++) {
    // Constraint 2
    lp.set_b(n_warehouses + stadium_index, stadium_demand[stadium_index]);
    lp.set_r(n_warehouses + stadium_index, CGAL::EQUAL);
    // Constraint 3
    lp.set_b(n_warehouses + n_stadiums + stadium_index,
        stadium_alcohol[stadium_index]);
  }

  for (int warehouse_index = 0; warehouse_index < n_warehouses;
      warehouse_index++) {
    // Constraint 1
    lp.set_b(warehouse_index, warehouse_supply[warehouse_index]);
    for (int stadium_index = 0; stadium_index < n_stadiums;
        stadium_index++) {
      lp.set_c(n_warehouses * warehouse_index + stadium_index,
          -revenues[warehouse_index][stadium_index]);
      // Constraint 1
      lp.set_a(n_warehouses * warehouse_index + stadium_index,
          warehouse_index, 1);
      // Constraint 2
      lp.set_a(n_warehouses * warehouse_index + stadium_index,
          n_warehouses + stadium_index, 1);
      // Constraint 3
      ET alcohol_volume = ET(warehouse_alcohol[warehouse_index]);
      // std::cout << "before division: " << alcohol_volume << std::endl;
      alcohol_volume /= 100;
      // std::cout << "after division: " << alcohol_volume << std::endl;
      lp.set_a(n_warehouses * warehouse_index + stadium_index,
          n_warehouses + n_stadiums + stadium_index, alcohol_volume);
    }
  }

  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
  if (s.is_infeasible()) {
    std::cout << "RIOT!" << std::endl;
  } else {
    // std::cout << CGAL::to_double(s.objective_value()) << std::endl;
    double res = floor_to_double(-s.objective_value());
    std::cout << std::setprecision(15) << res << std::endl;
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
