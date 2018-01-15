#include <algorithm>
#include <iostream>
// #include "prettyprint.hpp"
#include <vector>

std::vector<int> stack_size;
std::vector<std::vector<int> > chip_color;
std::vector<std::vector<std::vector<std::vector<std::vector<long> > > > > memo;

long get_score(std::vector<int> &heights, int n_stacks) {
  if (memo[heights[0]][heights[1]][heights[2]][heights[3]][heights[4]] != -1) {
    return memo[heights[0]][heights[1]][heights[2]][heights[3]][heights[4]];
  }
  // Check if at least two stacks are not depleted.
  int n_non_depleted = 0;
  for (auto h: heights) {
    if (h > 0) {
      n_non_depleted++;
    }
  }
  if (n_non_depleted < 2) {
    return 0;
  }

  long best_score = -1;
  // Test all 2^n_stacks subsets for color equality via bit shifting. The
  // binary representation of subset_index indicates which stacks are to be
  // included in the subset formation.
  for (int subset_index = 0; subset_index < 1 << n_stacks; subset_index++) {
    int color = 0;
    bool share_color = false;
    long score = 1;
    std::vector<int> new_heights(heights);
    for (int stack_index = 0; stack_index < n_stacks; stack_index++) {
      if (subset_index & 1 << stack_index) {
        // This stack is part of subset.
        if ((color == chip_color[stack_index][heights[stack_index]] or
            color == 0) and heights[stack_index] > 0) {
          color = chip_color[stack_index][heights[stack_index]];
          score *= 2;
          new_heights[stack_index]--;
          share_color = true;
        } else {
          share_color = false;
          break;
        }
      }
    }
    if (share_color) {
      // Adapt score to formula.
      best_score = std::max(best_score,
          score / 4 + get_score(new_heights, n_stacks));
    }
  }

  memo[heights[0]][heights[1]][heights[2]][heights[3]][heights[4]] = best_score;
  return best_score;
}

void test_case() {
  int n_stacks, color;
  stack_size = std::vector<int>(5, 0);
  chip_color = std::vector<std::vector<int> >(5);

  std::cin >> n_stacks;

  for (int stack_index = 0; stack_index < n_stacks; stack_index++) {
    std::cin >> stack_size[stack_index];
    chip_color[stack_index] = std::vector<int>(stack_size[stack_index] + 1, 0);
  }
  for (int stack_index = n_stacks; stack_index < 5; stack_index++) {
    chip_color[stack_index].push_back(0);
  }

  for (int stack_index = 0; stack_index < n_stacks; stack_index++) {
    for (int chip_index = 0; chip_index < stack_size[stack_index];
        chip_index++){
      std::cin >> color;
      chip_color[stack_index][chip_index + 1] = color;
    }
  }

  memo =
      std::vector<std::vector<std::vector<std::vector<std::vector<long> > > > >(
      stack_size[0] + 1, std::vector<std::vector<std::vector<
          std::vector<long> > > > (
      stack_size[1] + 1, std::vector<std::vector<std::vector<long> > >(
      stack_size[2] + 1, std::vector<std::vector<long> >(
      stack_size[3] + 1, std::vector<long>(
      stack_size[4] + 1, -1)))));

  std::vector<int> heights(stack_size);
  std::cout << get_score(heights, n_stacks) << std::endl;
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
