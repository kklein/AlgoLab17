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
    int left_wizard;
    for (int i = 0; i < n_wizards; i++) {
      cin >> lengths[i];
  	  cin >> rings[i];
  	  end_pos[i] = make_pair(i, rings[i] + lengths[i]);
      if (rings[i] < min_ring) {
        min_ring = rings[i];
        left_wizard = i;
      }
  	}
  	sort(end_pos.begin(), end_pos.end(), pair_compare);

    //for (int i = 0; i < n_wizards; i++) {
  	//  cout << end_pos[i].first << " : " << end_pos[i].second << endl;
  	//}

    int left = min_ring;
    int i = 0;
    while (i < n_wizards) {
      pair<int, int> p = end_pos[i];
      //cout << "looking at boat: " << p.first << endl;
      if (rings[p.first] >= left && p.first != left_wizard) {
        int min_end = 100000000;
        int min_pointer;
        if (rings[p.first] - left >= lengths[p.first]) {
          min_pointer = i;
          min_end = rings[p.first];
        } else {
          int j = i;
          pair<int, int> q = end_pos[j];
          int potential_right;
          while (rings[q.first] >= left  && j < n_wizards) {
            q = end_pos[j];
            potential_right = (left + lengths[q.first] > rings[q.first]) ? left + lengths[q.first] : rings[q.first];
            cout << "potential right: " << potential_right << endl;
            if (potential_right < min_end && q.first != left_wizard) {
              min_end = potential_right;
              min_pointer = j;
            }
            j++;
          }
        }
        cout << " took boat: " << end_pos[min_pointer].first << endl;
        i = min_pointer;
        left = min_end;
        n_placed_boats++;
      }
      i++;
    }

  	cout << n_placed_boats << endl;
  }
}
