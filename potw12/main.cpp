#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <boost/functional/hash.hpp>

// #include "prettyprint.hpp"

template <typename Container> // we can make this generic for any container [1]
struct container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};

void test_case() {
  int n_players, k;
  std::cin >> n_players >> k;
  std::vector<int> skills(n_players);

  for (int player_index = 0; player_index < n_players; player_index++) {
    std::cin >> skills[player_index];
  }

  long counter = 0;

  // This memoization attempt doesnt make a difference.
  std::map<std::vector<int>, std::map<int, int> > memo;

  for (int i = 0; i < (1 << n_players); i++) {
    std::bitset<22> bits(i);
    if (bits.count() < n_players - k) {
      continue;
    }

    long player_sum = 0;
    std::vector<int> first, second;
    int selected_index = 0;
    for (int player_index = 0; player_index < n_players; player_index++) {
      if (bits[player_index] == 0) {
        continue;
      }
      player_sum += skills[player_index];
      if (selected_index < bits.count() / 2) {
        first.push_back(skills[player_index]);
      } else {
        second.push_back(skills[player_index]);
      }
      selected_index++;
    }

    if (player_sum % 2 != 0) {
      continue;
    }

    std::map<int, int> sums;
    if (memo.find(first) != memo.end()) {
      sums = memo[first];
    } else {
      for (int j = 0; j < (1 << first.size()); j++) {
        std::bitset<22> first_bits(j);
        long sum = 0;
        for (int first_index = 0; first_index < first.size(); first_index++) {
          sum += (first_bits[first_index] == 1) ? first[first_index] : 0;
        }
        if (sums.find(sum) != sums.end()) {
          sums[sum]++;
        } else {
          sums[sum] = 1;
        }
      }
      memo[first] = sums;
    }

    for (int j = 0; j < (1 << second.size()); j++) {
      std::bitset<22> second_bits(j);
      long sum = 0;
      for (int second_index = 0; second_index < second.size(); second_index++) {
        sum += (second_bits[second_index] == 1) ? second[second_index] : 0;
      }
      long target = player_sum / 2 - sum;
      if (sums.find(target) != sums.end()) {
        counter += sums[target];
      }
    }
  }
  std::cout << counter << std::endl;
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
