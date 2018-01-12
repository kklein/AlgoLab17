#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
// #include "prettyprint.hpp"

void test_case() {
  std::ios_base::sync_with_stdio(false);
  int n_positions, n_transitions, r_start, b_start, position_a, position_b;
  std::cin >> n_positions >> n_transitions >> r_start >> b_start;
  // Use 0-indexing instead of 1-indexing.
  r_start--;
  b_start--;
  std::vector<std::vector<int> > adjacency(n_positions - 1, std::vector<int>());

  // Read DAG from input.
  for (int transition_index = 0; transition_index < n_transitions;
      transition_index++) {
    std::cin >> position_a >> position_b;
    // Use 0-indexing instead of 1-indexing.
    position_a--;
    position_b--;
    adjacency[position_a].push_back(position_b);
  }

  std::vector<int> next_step_shortest(n_positions - 1, INT_MAX);
  std::vector<int> next_step_longest(n_positions - 1, 0);
  for (int position = n_positions - 2; position >= 0; position--) {
    std::vector<int> reachable = adjacency[position];
    for (std::vector<int>::iterator it = reachable.begin();
        it != reachable.end(); ++it) {
      if (*it == n_positions - 1) {
        next_step_shortest[position] =
            std::min(next_step_shortest[position], 1);
        next_step_longest[position] = std::max(next_step_longest[position], 1);
      } else {
        next_step_shortest[position] = std::min(next_step_shortest[position],
            next_step_longest[*it] + 1);
        next_step_longest[position] = std::max(next_step_longest[position],
            next_step_shortest[*it] + 1);
      }
    }
  }

  int winner;
  if (next_step_shortest[r_start] < next_step_shortest[b_start]) {
    winner = 0;
  } else if (next_step_shortest[r_start] > next_step_shortest[b_start]) {
    winner = 1;
  } else if (next_step_shortest[r_start] % 2 == 1) {
    // Moriarty will be first if the number of required steps is even. Hence
    // Shorleck only has an edge (pun intended) if the number of required steps
    // is odd.
    winner = 0;
  } else {
    winner = 1;
  }
  std::cout << winner << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
}
