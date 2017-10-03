
#include <algorithm>
#include <vector>
#include <iostream>
#include <math.h>
#include <stack>

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
      pair<int, int> p = make_pair(ball, delay);
      balls[ball] = p;
      data[ball] = p;
    }
    sort(data.begin(), data.end(), compare);
    //for (int i = 0; i < n_balls; i++) {
    //  cout << data[i].first << " : " << data[i].second << endl;
    //}
    int i = 0;
    bool result = true;
    int taken_count = 0;
    while (result && taken_count < n_balls && i < n_balls) {
      //cout << "#taken: " << taken_count << endl;
      //cout << "i: " << i << endl;
      pair<int, int> p = data[i];
      if (taken[p.first] > 0) {
        i++;
      } else {
        stack<pair<int, int> > dependents;
        dependents.push(p);
        while (not dependents.empty()) {
          pair<int, int> p = dependents.top();
          dependents.pop();
          if (taken[p.first] == 0 ) {
            int child_pos = 2 * p.first + 1;
            if (child_pos < n_balls) {
              dependents.push(p);
              if (taken[child_pos] == 0) {
                dependents.push(balls[child_pos]);
              }
              if (taken[child_pos + 1] == 0) {
                dependents.push(balls[child_pos+ 1]);
              }
              if (taken[child_pos] > 0 and taken[child_pos + 1] > 0) {
                p = dependents.top();
                dependents.pop();
                if (p.second > taken_count) {
                  //cout << "take " << p.first << " with ti: " << p.second << " at time: " << taken_count
                  //    << endl;
                  taken[p.first] = 1;
                  taken_count++;
                } else {
                  result = false;
                }
              }
            } else {
              if (p.second > taken_count) {
                //cout << "take " << p.first << " with ti: " << p.second << " at time: " << taken_count
                //    << endl;
                taken[p.first] = 1;
                taken_count++;

              } else {
                result = false;
              }
            }
          }
        }
      }
    }
    cout << (result ? "yes" : "no") << endl;
  }
  return 0;
}
