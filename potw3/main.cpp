#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool segment_compare (pair<int, int> p, pair<int, int> q) {
  return (p.second < q.second or (p.second == q.second and p.first > q.first)); 
}

//bool less_or_equal (int a, int b) {
//  return 
//}

int main() {
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_jedis;
    int n_segments;
    cin >> n_jedis;
    cin >> n_segments;
    int segment_start;
    int segment_end;
    vector<pair<int, int> > segments(n_jedis);
    for (int jedi = 0; jedi < n_jedis; jedi++) {
      cin >> segment_start;
      cin >> segment_end;
      segments[jedi] = make_pair(segment_start, segment_end);
    }
    sort(segments.begin(), segments.end(), segment_compare);
    int count = 1;
    int k = 0;
    for (int jedi = 1; jedi < n_jedis; jedi++) {
      if (segments[jedi].first > segments[k].second) {
        count++;
        k = jedi;
      }
    }
    cout << count << endl;
  }
}
