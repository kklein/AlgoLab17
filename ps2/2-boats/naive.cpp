#include <algorithm>
#include <iostream>
#include <vector>

using  namespace  std;

bool pair_compare(pair<int, int> p, pair<int, int> q) {
  return (p.second < q.second);
}

int  main (void) {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_wizards;
  	int n_placed_boats = 1;
  	cin >> n_wizards;
  	vector<int> rings(n_wizards);
  	vector<int> lengths(n_wizards);
  	vector<pair<int, int> > end_pos(n_wizards);
    int min_ring = 100000000;
    int max_ring = -1;
    int left_wizard;
    for (int i = 0; i < n_wizards; i++) {
      cin >> lengths[i];
  	  cin >> rings[i];
  	  end_pos[i] = make_pair(i, rings[i] + lengths[i]);
      if (rings[i] < min_ring) {
        min_ring = rings[i];
        left_wizard = i;
      }
      if (rings[i] > max_ring) {
        max_ring = rings[i];
        right_wizard = i;
      }
    }
    cout << n_placed_boats << endl;
}
