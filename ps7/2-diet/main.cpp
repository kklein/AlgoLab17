#include <cassert>
#include <iostream>
#include <vector>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// Exact integral type.
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// We minimize the sum of (quantity, price) products per food item.
// The quantity per food item cannot be negative.
// The sum of (quantity, nutrients) products per food item has to be at least as big as the min.
// The sum of (quantity, -nutrients) products per food item hast to be at least as big as the negative max.

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  Program lp (CGAL::LARGER, true, 0, false, 0);
  int min, max, n_nutrients, n_foods;
  std::cin >> n_nutrients >> n_foods;
  while (n_nutrients != 0 or n_foods != 0) {
    int price, nutrient_value;
    for (int nutrient_index = 0; nutrient_index < n_nutrients; nutrient_index++) {
      std::cin >> min >> max;
      lp.set_b(nutrient_index * 2, min);
      lp.set_b(nutrient_index * 2 + 1, -max);
    }
    for (int food_index = 0; food_index < n_foods; food_index++) {
      std::cin >> price;
      lp.set_c(food_index, price);
      for (int nutrient_index = 0; nutrient_index < n_nutrients; nutrient_index++) {
        std::cin >> nutrient_value;
        lp.set_a(food_index, nutrient_index * 2, nutrient_value);
        lp.set_a(food_index, nutrient_index * 2 + 1, - nutrient_value);
      }
    }
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()) {
      std::cout << floor_to_double(s.objective_value()) << std::endl;
      std::cout << *(s.optimality_certificate_begin()) << std::endl;
    } else {
      std::cout << "No such diet." << std::endl;
    }

    std::cin >> n_nutrients >> n_foods;
  }
  return 0;
}
