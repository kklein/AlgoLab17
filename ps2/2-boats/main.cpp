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
  	int n_placed_boats = 0;
  	cin >> n_wizards;
  	vector<int> rings(n_wizards);
  	vector<int> lengths(n_wizards);
  	vector<pair<int, int> > start_pos(n_wizards);
  	int max_ring = 0;
    for (int i = 0; i < n_wizards; i++) {
      cin >> lengths[i];
  	  cin >> rings[i];
  	  max_ring = max(max_ring, rings[i]);
  	  start_pos[i] = make_pair(i, rings[i] - lengths[i]);
  	}
  	sort(start_pos.begin(), start_pos.end(), pair_compare);
  	for (int i = 0; i < n_wizards; i++) {
  	  cout << start_pos[i].first << " : " << start_pos[i].second << endl;
  	}

    int left_pointer = -100000;
  	for (int i = 0; i < n_wizards; i++) {
      pair<int, int> p = start_pos[i];
      if (rings[p.first] - left_pointer >= lengths[p.first]) {
        n_placed_boats++;
        left_pointer = rings[p.first];
        cout << " took boat: " << p.first << endl;
      }
  	}

  	cout << n_placed_boats << endl;
  }
}
