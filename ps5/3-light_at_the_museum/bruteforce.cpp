#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

using namespace std;

vector<int> add_to_state(vector<int> state, int switch_index, int on_off,
    vector<vector<vector<int> > > &switch_effects) {
  for (int room_index = 0; room_index < state.size(); room_index++) {
    state[room_index] += switch_effects[switch_index][room_index][on_off];
  }
  return state;
}

bool state_is_desirable(vector<int> state, vector<int> &required_lights) {
  bool is_desirable = true;
  for (int room_index = 0; room_index < state.size(); room_index++) {
    is_desirable =
        is_desirable and state[room_index] == required_lights[room_index];
  }
  return is_desirable;
}

int foo(vector<int> state, int n_switched_switches, int next,
    vector<vector<vector<int> > > &switch_effects, vector<int> &required_lights)
    {
  vector<int> state0 = add_to_state(state, next, 0, switch_effects);
  vector<int> state1 = add_to_state(state, next, 1, switch_effects);
  if (next == switch_effects.size() - 1 ) {
    if (state_is_desirable(state0, required_lights)) {
      return n_switched_switches;
    } else if (state_is_desirable(state1, required_lights)) {
      return n_switched_switches + 1;
    } else {
      return -1;
    }
  } else {
    int result0 = foo(state0, n_switched_switches, next + 1, switch_effects,
        required_lights);
    int result1 = foo(state1, n_switched_switches + 1, next + 1,
        switch_effects, required_lights);
    if (result0 == -1 and result1 == -1) {
      return -1;
    } else if (result0 == -1) {
      return result1;
    } else if (result1 == -1) {
      return result0;
    } else {
      return min(result0, result1);
    }
  }
}

void test_case() {
  int n_switches, n_rooms;
  cin >> n_switches >> n_rooms;
  vector<int> required_lights(n_rooms);
  for (int room_index = 0; room_index < n_rooms; room_index++) {
    cin >> required_lights[room_index];
  }
  vector<vector<vector<int> > > switch_effects(n_switches,
      vector<vector<int > >(n_rooms, vector<int>(2)));
  for (int switch_index = 0; switch_index < n_switches; switch_index++) {
    for (int room_index = 0; room_index < n_rooms; room_index++) {
      cin >> switch_effects[switch_index][room_index][0] >>
          switch_effects[switch_index][room_index][1];
    }
  }

  vector<int> state(n_rooms, 0);
  int result = foo(state, 0, 0, switch_effects, required_lights);
  if (result == -1) {
    std::cout << "impossible" << std::endl;
  } else {
    std::cout << result << std::endl;
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
