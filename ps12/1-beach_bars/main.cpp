#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

void test_case() {
  int n_customers;
  std::cin >> n_customers;
  std::vector<int> positions(n_customers);
  for (int customer_index = 0; customer_index < n_customers; customer_index++) {
    std::cin >> positions[customer_index];
  }

  std::sort(positions.begin(), positions.end());

  int highest_position = positions[positions.size() - 1];
  int n_possible_spots = highest_position - positions[0] + 1;

  std::vector<int> left_most_reachable(n_possible_spots), right_most_reachable(n_possible_spots), locations(n_possible_spots);
  for (int i = 0; i < n_possible_spots; i++) {
    locations[i] = positions[0] + i;
  }

  int current_spot = 0, left = 0, right = 0;
  while (right < positions.size() and
      positions[right] - locations[current_spot] <= 100) {
    right++;
  }
  right--;
  left_most_reachable[current_spot] = left;
  right_most_reachable[current_spot] = right;
  int max_span = right - left + 1;
  int min_distance_max_span = positions[right] - locations[current_spot];
  std::vector<int> solutions;
  solutions.push_back(current_spot);

  for (current_spot = 1; current_spot < n_possible_spots; current_spot++) {
    left = left_most_reachable[current_spot - 1];
    right = right_most_reachable[current_spot - 1];
    while (locations[current_spot] - positions[left] > 100) {
      left++;
    }
    right++;
    while (right < positions.size() and positions[right] - locations[current_spot] <= 100) {
      right++;
    }
    right--;
    assert(right < positions.size());
    left_most_reachable[current_spot] = left;
    right_most_reachable[current_spot] = right;
    int span = right - left + 1;
    int distance = std::max(locations[current_spot] - positions[left],
        positions[right] - locations[current_spot]);
    if (span > max_span or
        span == max_span and distance < min_distance_max_span) {
      solutions.clear();
      solutions.push_back(current_spot);
      max_span = span;
      min_distance_max_span = distance;
    } else if (span == max_span and distance == min_distance_max_span) {
      solutions.push_back(current_spot);
    }
  }

  std::cout << max_span << " " << min_distance_max_span << std::endl;
  for (int solution: solutions) {
    std:: cout << locations[solution] << " ";
  }
  std::cout << std::endl;
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
