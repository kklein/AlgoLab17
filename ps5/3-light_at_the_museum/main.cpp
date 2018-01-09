#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;
int is_possible(int index, vector<int> &state,
    vector<vector<vector<int> > > &switch_effects) {
  if (index == 0) {
    bool result1 = true;
    bool result2 = true;
    for (int room_index = 0; room_index < state.size(); room_index++) {
      result1 = result1 and state[room_index] == switch_effects[0][room_index][0];
      result2 = result2 and state[room_index] == switch_effects[0][room_index][1];
    }
    if (result1) {
      return 0;
    } else if (result2) {
      return 1;
    } else {
      return -1;
    }
  }
  vector<int> taken(state.size());
  vector<int> negated(state.size());
  for (int room_index = 0; room_index < state.size(); room_index++) {
    taken[room_index] =
        state[room_index] - switch_effects[index][room_index][0];
    negated[room_index] =
        state[room_index] - switch_effects[index][room_index][1];
  }
  if (equal(taken.begin(), taken.begin() + taken.size(), negated.begin())) {
    cout << "mistake!" << endl;
  }
  int result1 = is_possible(index - 1, taken, switch_effects);
  result1 = (result1 >= 0) ? result1 : INT_MAX - 1;
  int result2 = is_possible(index - 1, negated, switch_effects);
  result2 = (result2 >= 0) ? result2 : INT_MAX - 1;
  if (min(result1, result2) == INT_MAX - 1) {
    return -1;
  } else {
    return min(result1, result2 + 1);
  }
}

void test_case() {
  int n_switches, n_rooms;
  cin >> n_switches >> n_rooms;
  vector<int> required_lights(n_rooms);
  for (int room_index = 0; room_index < n_rooms; room_index++) {
    cin >> required_lights[room_index];
  }
  vector<vector<vector<int> > > switch_effects(n_switches, vector<vector<int > >(n_rooms, vector<int>(2)));
  for (int switch_index = 0; switch_index < n_switches; switch_index++) {
    for (int room_index = 0; room_index < n_rooms; room_index++) {
      cin >> switch_effects[switch_index][room_index][0] >>
          switch_effects[switch_index][room_index][1];
    }
  }
  int result = is_possible(n_switches - 1, required_lights, switch_effects);
  if (result == -1) {
      cout << "impossible" << endl;
  } else {
    cout << result << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_index = 0; test_index < n_test_cases; test_index++) {
    test_case();
  }
  return 0;
}
