#include <algorithm>
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Quotient<ET> SolT;

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;


// round up to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

// Constraint 1: sum_j q(w_i, s_j) * 1 <= supply(w_i)
// Constraint 2: sum_i q(w_i, s_j) * 1 = demand(s_j)
// Constraint 3: sum_i q(q_i, s_j) * alcohol(w_i) / 100 <= alcohol(s_j)

void test_case() {
  int n_warehouses, n_stadiums, n_lines;
  std::cin >> n_warehouses >> n_stadiums >> n_lines;

  std::vector<Point> warehouse_location(n_warehouses),
      stadium_location(n_stadiums), line_center(n_lines);
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
    std::cin >> stadium_location[stadium_index] >> stadium_demand[stadium_index]
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

  // Create triangulation of warehouses and stadiums.
  std::vector<Point> objects(n_warehouses + n_stadiums);
  for (int warehouse_index = 0; warehouse_index < n_warehouses;
      warehouse_index++) {
    objects[warehouse_index] = warehouse_location[warehouse_index];
  }
  for (int stadium_index = 0; stadium_index < n_stadiums; stadium_index++) {
    objects[n_warehouses + stadium_index] = stadium_location[stadium_index];
  }
  Triangulation t;
  t.insert(objects.begin(), objects.end());

  std::vector<int> relevant_lines;
  for (int line_index = 0; line_index < n_lines; line_index++) {
    std::cin >> line_center[line_index] >> line_radius[line_index];
    Point p = t.nearest_vertex(line_center[line_index]) -> point();
    if (CGAL::squared_distance(p, line_center[line_index]) <
        line_radius[line_index] * line_radius[line_index]) {
      relevant_lines.push_back(line_index);
    }
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

  std::vector<std::vector<int> > additional_cost(n_warehouses,
      std::vector<int>(n_stadiums, 0));
  for (int line_index = 0; line_index < relevant_lines.size(); line_index++) {
    K::FT squared_radius = line_radius[line_index] * line_radius[line_index];
    Point p = line_center[line_index];
    for (int warehouse_index = 0; warehouse_index < n_warehouses;
        warehouse_index++) {
      K::FT squared_warehouse_distance =
          CGAL::squared_distance(warehouse_location[warehouse_index], p);
      for (int stadium_index = 0; stadium_index < n_stadiums;
          stadium_index++) {
        K::FT squared_stadium_distance =
          CGAL::squared_distance(stadium_location[stadium_index], p);
        if ((squared_warehouse_distance < squared_radius and
            squared_stadium_distance > squared_radius) or
            (squared_warehouse_distance > squared_radius and
            squared_stadium_distance < squared_radius)) {
          additional_cost[warehouse_index][stadium_index]++;
        }
      }
    }
  }

  for (int warehouse_index = 0; warehouse_index < n_warehouses;
      warehouse_index++) {
    // Constraint 1
    lp.set_b(warehouse_index, warehouse_supply[warehouse_index]);
    for (int stadium_index = 0; stadium_index < n_stadiums;
        stadium_index++) {
      lp.set_c(n_warehouses * warehouse_index + stadium_index,
          ET(-revenues[warehouse_index][stadium_index]) + ET(additional_cost[warehouse_index][stadium_index]) / 100);
      // Constraint 1
      lp.set_a(n_warehouses * warehouse_index + stadium_index,
          warehouse_index, 1);
      // Constraint 2
      lp.set_a(n_warehouses * warehouse_index + stadium_index,
          n_warehouses + stadium_index, 1);
      // Constraint 3
      ET alcohol_volume = ET(warehouse_alcohol[warehouse_index]);
      alcohol_volume /= 100;
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
