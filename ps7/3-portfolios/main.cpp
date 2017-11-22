#include <iostream>
#include <vector>

#include <CGAL/basic.h>
#include <CGAL/Gmpq.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// TODO(kevinkle): Test whether lp is 'reusable', i.e. a change of b suffices to create a different lp.
int main() {
  std::ios_base::sync_with_stdio(false);
  int n_assets, n_clients, max_cost, min_return, max_variance;
  std::cin >> n_assets >> n_clients;
  while (n_assets > 0 and n_clients > 0) {
    std::vector<int> costs(n_assets);
    std::vector<int> returns(n_assets);
    std::vector<std::vector<int> > covariances(n_assets, std::vector<int>(n_assets));
    for (int asset_index = 0; asset_index < n_assets; asset_index++) {
      std::cin >> costs[asset_index] >> returns[asset_index];
    }
    for (int asset_index = 0; asset_index < n_assets; asset_index++) {
      for (int asset_index2 = 0; asset_index2 < n_assets; asset_index2++) {
        std::cin >> covariances[asset_index][asset_index2];
      }
    }
    for (int client_index = 0; client_index < n_clients; client_index++) {
      // Maximum cost, minimum expected return, maximum variance.
      std::cin >> max_cost >> min_return >> max_variance;
      Program qp (CGAL::SMALLER, true, 0, false, 0);
      for (int asset_index = 0; asset_index < n_assets; asset_index++) {
        qp.set_a(asset_index, 0, costs[asset_index]);
        qp.set_a(asset_index, 1, -returns[asset_index]);
        for (int asset_index2 = 0; asset_index2 < n_assets; asset_index2++) {
          qp.set_d(asset_index, asset_index2, 2*covariances[asset_index][asset_index2]);
        }
      }
      qp.set_b(0, max_cost);
      qp.set_b(1, -min_return);
      Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
      if (s.is_optimal() and s.objective_value() <= max_variance) {
        std::cout << "Yes." << std::endl;
      } else {
        std::cout << "No." << std::endl;
      }
    }
    std::cin >> n_assets >> n_clients;
  }
  return 0;
}
