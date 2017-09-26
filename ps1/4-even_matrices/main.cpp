#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int size;
    cin >> size;
    int input;
    vector<vector<int> > matrix(size, vector<int>(size));
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        cin >> input;
        matrix[i][j] = input;
      }
    }

    int even_count = 0;

    // Build sum matrix.
    int offset;
    vector<vector<int> > sums(size, vector<int>(size));
    sums[0][0] = matrix[0][0];
    if (sums[0][0] % 2 == 0) {
      even_count++;
    }
    for (int j = 1; j < size; j++) {
      sums[0][j] = sums[0][j-1] + matrix[0][j];
      if (sums[0][j] % 2 == 0) {
        even_count++;
      }
    }
    for (int i = 1; i < size; i++) {
      sums[i][0] = sums[i-1][0] + matrix[i][0];
      if (sums[i][0] % 2 == 0) {
        even_count++;
      }
      for (int j = 1; j < size; j++) {
        offset = sums[i-1][j] - sums[i-1][j-1];
        sums[i][j] = sums[i][j-1] + offset + matrix[i][j];
        if (sums[i][j] % 2 == 0) {
          even_count++;
        }
      }
    }

    // Count even sum differences.
    int i1;
    int j1;
    int sum;
    for (int i2 = 0; i2 < size; i2++) {
      for (int j2 = 0; j2 < size; j2++) {
        for (i1 = 0; i1 <= i2; i1++) {
          for (j1 = 0; j1 <= j2; j1++) {
            if ((i1 != i2 && j1 != j2)) {
              sum = sums[i2][j2] - sums[i2][j1] - sums[i1][j2] + sums[i1][j1];
              if (sum % 2 == 0) {
                even_count++;
              }
            } else if (i1 != i2 || j1 != j2) {
              sum = sums[i2][j2] - sums[i1][j1];
              if (sum % 2 == 0) {
                even_count++;
              }
            }
          }
        }
      }
    }
    cout << even_count << endl;
  }
  return 0;
}
