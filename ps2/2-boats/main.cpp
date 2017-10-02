#include <algorithm>
#include <iostream>
#include <pair>
#include <vector>

using  namespace  std;

bool pairCompare(pair<int, int> p, pair<int, int> q) {
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
	vector<pair<int, int> > ends(n_wizards);
	int max_ring = 0;
    for (int i = 0; i < n_wizards; i++) {
	  cin << rings[i];
	  cin << lengths[i];
	  max_ring = max(max_ring, rings);
	  start_pos[i] = make_pair(i, rings[i] - lengths[i]);
	}
	
	sort(start_pos.begin(), start_pos.end(), pairCompare);
	for (int i = 0; i < n_wizards; i++) {
	  cout << start_pos[i].first << " : " < start_pos[i].second << endl;
	}
	
	// represents the number of boats placed in this coordinate
	vector<int> positions(max_rings + 1);
	for (int i = 0; i < max_rings + 1) {
	  
	}
	
	cout << n_placed_boats << endl;
  }
}