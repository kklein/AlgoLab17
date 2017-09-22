#include <iostream>
#include <vector>

using namespace std;

// 1 means equal
// 2 means possibly too light
// 3 means possibly too heavy

void set_values(vector<int>& coins, vector<int> v, int value) {
  if (value == 1) {
    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
      coins[*it - 1] = 1;
      if (*it-1 == 182) {cout << "183 has been set to 1" << endl;}
    }
    return;
  }
  int actual_value;
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    actual_value = coins[*it - 1];
    
    if (actual_value == 0) {
      coins[*it - 1] = value;
      if (*it-1 == 182) {cout << "183 has been set to"; cout << value << endl;}
    } else if (actual_value == 1 || (actual_value == 2 && value == 3) || 
        (actual_value == 3 && value == 2)) {
      coins[*it - 1] = 1;
      if (*it-1 == 182) {cout << "183 has been set to 1"<< endl;}
    }
  }
}

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_coins;
    int n_weighings;
    int n_coins_per_side;
    char op;
    cin >> n_coins;
    cin >> n_weighings;
    vector<int> coins(n_coins);
    for (int weighing = 0; weighing < n_weighings; weighing++) {
      cin >> n_coins_per_side;
      vector<int> left(n_coins_per_side);
      vector<int> right(n_coins_per_side);
      for (int i = 0; i < n_coins_per_side; i++) {
        cin >> left[i];
      }
      for (int i = 0; i < n_coins_per_side; i++) {
        cin >> right[i];
      }
      cin >> op;
      switch (op) {
        case '=' :
          set_values(coins, left, 1);
          set_values(coins, right, 1);
          break;
        case '<' :
          set_values(coins, left, 2);
  	  set_values(coins, right, 3);
          break;
        case '>' :
          set_values(coins, left, 3);
          set_values(coins, right, 2);
          break;
      }
    }

    int candidate;
    int candidate_count = 0;
    for (int i = 0; i < n_coins; i++) {
      cout << i;
      cout << ':';
      cout << coins[i] << endl;
      if (coins[i] != 1) {
        candidate = i + 1;
        candidate_count++;
      }
    }

    if (candidate_count == 1) {
      cout << candidate;
    } else {
      cout << 0;
    }
    cout << "number of candidates" << endl;
    cout << candidate_count << endl;
    cout << "most recent candidate" << endl;
    cout << candidate << endl;

  }
  return 0;
}
