#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

// #include "prettyprint.hpp"

std::vector<int> left_segment_indeces;
std::vector<int> left_segment_sums;

int set_left_segment_index_and_sum(int defender_index, int strength,
    std::vector<int> &defenses) {
  if (left_segment_indeces[defender_index] == -2) {
    int left_segment_index = defender_index;
    int segment_sum = defenses[defender_index];
    while (segment_sum < strength and left_segment_index > 0) {
      left_segment_index--;
      segment_sum += defenses[left_segment_index];
    }
    left_segment_indeces[defender_index] = left_segment_index;
    left_segment_sums[defender_index] = segment_sum;
  }
}

void test_case() {
  int n_defenders, n_attackers, strength;
  std::cin >> n_defenders >> n_attackers >> strength;

  left_segment_indeces = std::vector<int>(n_defenders, -2);
  left_segment_sums = std::vector<int>(n_defenders, -2);

  std::vector<std::vector<int> >
      table(n_defenders, std::vector<int>(n_attackers));
  std::vector<int> defenses(n_defenders);
  for (int defender_index = 0; defender_index < n_defenders; defender_index++) {
    std::cin >> defenses[defender_index];
  }

  table[0][0] = defenses[0] == strength ? 1 : 0;

  for (int defender_index = 1; defender_index < n_defenders; defender_index++) {
    set_left_segment_index_and_sum(defender_index, strength, defenses);
    int left_segment_index = left_segment_indeces[defender_index];
    int segment_sum = left_segment_sums[defender_index];
    int n_attacked_defenders = defender_index - left_segment_index + 1;
    int not_taken = table[defender_index - 1][0];
    if (left_segment_index >= 0 and segment_sum == strength and
        n_attacked_defenders > not_taken) {
      table[defender_index][0] = n_attacked_defenders;
    } else {
      table[defender_index][0] = not_taken;
    }
  }

  for (int attacker_index = 1; attacker_index < n_attackers; attacker_index++) {
    table[attacker_index][0] = table[attacker_index - 1][0];
    for (int defender_index = 1; defender_index < n_defenders;
        defender_index++) {
      int left_segment_index = left_segment_indeces[defender_index];
      int segment_sum = left_segment_sums[defender_index];
      int n_attacked_defenders = defender_index - left_segment_index + 1;
      if (left_segment_index - 1 >= 0) {
        n_attacked_defenders +=
            table[left_segment_index - 1][attacker_index - 1];
      }
      int not_taken = table[defender_index - 1][attacker_index];
      if (left_segment_index >= 0 and segment_sum == strength and
          n_attacked_defenders > not_taken) {
        table[defender_index][attacker_index] = n_attacked_defenders;
      } else {
        table[defender_index][attacker_index] = not_taken;
      }
    }
  }

  // std::cout << table << std::endl;

  if (table[n_defenders - 1][n_attackers - 1] !=
      table[n_defenders - 1][n_attackers - 2]) {
    std::cout << table[n_defenders - 1][n_attackers - 1] << std::endl;
  } else {
    std::cout << "fail" << std::endl;
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
