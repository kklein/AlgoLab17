#include <iostream>

using namespace std;

int main (void) {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int input_length;
    cin >> input_length;
    double sum = 0;
    double summand;
    for (int i = 0; i < input_length; i++) {
      cin >> summand;
      sum += summand;
    }
  cout << sum << endl;
  }
  return 0;
}
