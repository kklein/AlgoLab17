#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int size_i;
    int size_j;
    cin >> size_i;
    cin >> size_j;
    int input;
    vector<vector<int> > matrix(size_i, vector<int>(size_j));
    for (int i = 0; i < size_i; i++) {
      for (int j = 0; j < size_j; j++) {
        cin >> input;
        matrix[i][j] = input;
      }
    }
    
    vector<vector<int> > row_sums(size_i, vector<int>(size_j));
    vector<int> row_even_count(size_i);
    vector<int> row_odd_count(size_i);
    int row_sum;
    for (int i = 0; i < size_i; i++) {
      row_sum = 0;
      row_even_count[i] = 0;
      row_odd_count[i] = 0;
      for (int j = 0; j < size_j; j++) {
        row_sum += matrix[i][j];
        row_sums[i][j] = row_sum;
        if (row_sum % 2 == 0) {
          row_even_count[i]++;
        } else {
          row_odd_count[i]++;
        }
      }
    }

    vector<vector<int> > column_sums(size_i, vector<int>(size_j));
    vector<int> column_even_count(size_j);
    vector<int> column_odd_count(size_j);
    int column_sum;
    for (int j = 0; j < size_j; j++) {
      column_sum = 0;
      column_even_count[i] = 0;
      column_odd_count[i] = 0;
      for (int i = 0; i < size_i; i++) {
        column_sum += matrix[i][j];
 	column_sums[i][j] = column_sum;
        if (column_sum % 2 == 0) {
          column_even_count[j]++;
        } else {
          column_odd_count[j]++;
        }
      }
    }

    int even_count = 0;
    
    cout << even_count << endl;
  }
  return 0;
}
