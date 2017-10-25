#include <iostream>
#include <vector>

using namespace std;

void test() {
  int n_coins;
  cin >> n_coins;
  vector<int, int> values(n_coins);
  for (int coin = 0; coin < n_coins; ++coin) {
    cin >> values[coin];
  }
}

int main(){
  int n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; ++test_case) {
    test();
  }
  return 0;
}
