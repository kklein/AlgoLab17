#include <algorithm>
#include <iostream>
#include <vector>
// #include "prettyprint.hpp"

void test_case() {
  int n_positions, n_transitions, r_start, b_start, position_a, position_b;
  std::cin >> n_positions >> n_transitions >> r_start >> b_start;
  std::vector<std::vector<int> > adjacency(n_positions - 1, std::vector<int>());
  for (int transition_index = 0; transition_index < n_transitions;
      transition_index++) {
    std::cin >> position_a >> position_b;
    // Use 0-indexing instead of 1-indexing.
    position_a--;
    position_b--;
    adjacency[position_a].push_back(position_b);
  }

  // This table is 3-dimensional. The first dimension expresses the next action
  // in the game. 0 stands for (Sherlock, even), 1 for (Moriarty, even), 2 for
  // (Sherlock, odd) and 4 for (Moriarty, odd). Hence action 0 moves red and
  // seeks to minimize (i.e. reach 0), action 1 moves blue and seeks to
  // maximize (i.e. reach 1), 2 moves blue and seeks to minimize and 3 moves red
  // and seeks to maximize.
  std::vector<std::vector<std::vector<int> > > t(4,
      std::vector<std::vector<int> >(n_positions - 1,
      std::vector<int>(n_positions - 1, -1)));

  // Computation of table content from lowest to highest row, from right to
  // left.
  for (int position_r = n_positions - 2; position_r >= 0; position_r--) {
    for (int position_b = n_positions - 2; position_b >= 0; position_b--) {
      int score_0 = 1; int score_1 = 0; int score_2 = 1; int score_3 = 0;
      std::vector<int> reachable = adjacency[position_r];
      for(std::vector<int>::iterator it = reachable.begin();
          it != reachable.end(); ++it) {
        if (*it == n_positions - 1) {
          score_0 = 0;
          score_3 = std::max(score_3, 0);
        } else {
          score_0 = std::min(score_0, t[1][*it][position_b]);
          score_3 = std::max(score_3, t[0][*it][position_b]);
        }
      }
      reachable = adjacency[position_b];
      for(std::vector<int>::iterator it = reachable.begin();
          it != reachable.end(); ++it) {
        if (*it == n_positions - 1) {
          score_1 = 1;
          score_2 = std::min(score_2, 1);
        } else {
          score_1 = std::max(score_1, t[2][position_r][*it]);
          score_2 = std::min(score_2, t[3][position_r][*it]);
        }
      }
      t[0][position_r][position_b] = score_0;
      t[1][position_r][position_b] = score_1;
      t[2][position_r][position_b] = score_2;
      t[3][position_r][position_b] = score_3;
    }
  }

  // std::cout << t << std::endl;
  std::cout << t[0][r_start][b_start] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n_test_cases;
  std::cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
}
