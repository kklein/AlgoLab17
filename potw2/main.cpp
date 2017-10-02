
#include <algorithm>
#include <vector>
#include <iostream>
#include <math.h>
#include <queue>

using namespace std;

bool comparison(pair<int, int> p, pair <int, int> q) {
  float height_p = floor(log2(p.first + 1));
  float height_q = floor(log2(q.first + 1));
  return (height_p < height_q || (height_p == height_q && p.second > q.second));
}

bool sort_comparison(pair<int, int> p, pair <int, int> q) {
  float height_p = floor(log2(p.first + 1));
  float height_q = floor(log2(q.first + 1));
  return (height_q < height_p || (height_p == height_q && p.second < q.second));
}

bool compare(pair<int, int> p, pair <int, int> q) {
  return p.second < q.second;
}

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_balls;
    int delay;
    cin >> n_balls;
    vector<int> taken(n_balls, 0);
    vector<pair<int, int> > data(n_balls);
    vector<pair<int, int> > balls(n_balls);
    for (int ball = 0; ball < n_balls; ball++) {
      cin >> delay;
      balls[balls] = make_pair(ball, delay);
      data[ball] = make_pair(ball, delay);
    }
    sort(data.begin(), data.end(), compare);
    for (int i = 0; i < n_balls; i++) {
      cout << data[i].first << " : " << data[i].second << endl;
    }
    int i = 0;
    bool result = true;
    int taken_count = 0;
    while (result && taken_count < n_balls && i < n_balls) {
      pair<int, int> p = data[i];
      if (taken[p.first] > 1) {
        i++;
      } else {
        if (p.second > taken_count) {
          result = false;
        } else {
          queue<pair<int, int> > dependents;
          dependents.push(p);
          while (not dependents.empty()) {
            v = dependents.front();
            dependents.pop();
            if (taken[v.first] == 0 ) {
              if (v.second > taken_count) {
                taken[v.first] = 1;
                taken_count++;
                if (taken[2*j + 1] < n_balls) {
                  dependents.push(balls[2*j + 1]);
                  dependents.push(balls[2*j + 2]);
                }
              } else {
                result = false;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}
