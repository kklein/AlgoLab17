#include <algorithm>
#include <iostream>
#include <vector>

void test_case() {
  int n_drinks, n_guests;
  std::cin >> n_drinks >> n_guests;
  std::vector<int> unit_quantities(n_drinks);
  std::vector<int> unit_prices(n_drinks);

  for (int drink_index = 0; drink_index < n_drinks; drink_index++) {
    std::cin >> unit_prices[drink_index] >> unit_quantities[drink_index];
  }

  std::vector<std::vector<std::pair<int, int> > > table(n_drinks,
      std::vector<std::pair<int, int> >(n_guests + 1));

  table[0][0] = std::make_pair(0, 0);

  for (int quantity = 1; quantity < n_guests + 1; quantity++) {
    int cost = unit_prices[0] * ((quantity + unit_quantities[0] - 1) /
        unit_quantities[0]);
    table[0][quantity] = std::make_pair(cost, 1);
  }

  for (int drink_index = 1; drink_index < n_drinks; drink_index++) {
    table[drink_index][0] = table[drink_index - 1][0];
    for (int quantity = 1; quantity < n_guests + 1; quantity++) {
      auto q = table[drink_index - 1][quantity];
      int min_cost, drink_count;
      if (quantity < unit_quantities[drink_index]) {
        min_cost = unit_prices[drink_index];
        drink_count = 1;
      } else {
        std::pair<int, int> p =
            table[drink_index][quantity - unit_quantities[drink_index]];
        min_cost = p.first + unit_prices[drink_index];
        drink_count = p.second +
            (table[drink_index - 1][quantity - unit_quantities[drink_index]] ==
            table[drink_index][quantity - unit_quantities[drink_index]] ?
            1 : 0);
      }
      if (q.first < min_cost) {
        min_cost = q.first;
        drink_count = q.second;
      } else if (q.first == min_cost){
        drink_count = std::max(drink_count, q.second);
      }
      table[drink_index][quantity] = std::make_pair(min_cost, drink_count);
    }
  }

  std::cout << table[n_drinks - 1][n_guests].first << " " <<
      table[n_drinks - 1][n_guests].second << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
}
