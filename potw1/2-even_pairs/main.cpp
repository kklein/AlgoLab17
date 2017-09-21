#include <iostream>
#include <vector>

using namespace std;

int n_choose_2(int n) {
  return n * (n-1) / 2;
}

int main(void) {
  int n_test_cases;
  cin >> n_test_cases;
  int input_length;
  int aux;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    cin >> input_length;
    vector<int> input(input_length);
    vector<int> sums(input_length);
    for (int i = 0; i < input_length; i++) {
      cin >> aux;
      input[i] = aux;
    }
    int counter_even = 0;
    int counter_odd = 0;
    int sum = 0;
    for (int i = 0; i < input_length; i++) {
      sum += input[i];
      sums[i] = sum;
      if (sum % 2 == 0) {
	counter_even++;
      } else {
        counter_odd++;
      }
    } 
    int counter = n_choose_2(counter_even) + n_choose_2(counter_odd) + counter_even;
    cout << counter << endl;
  }
  return 0;
}
