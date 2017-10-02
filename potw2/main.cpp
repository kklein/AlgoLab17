
#include <algorithm> 
#include <vector>
#include <iostream>
#include <math.h> 

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

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    int n_balls;
    cin >> n_balls;
    vector<int> delays(n_balls);
    vector<int> lost_children(n_balls); 
    vector<pair<int, int> > min_heap(n_balls);
    for (int ball = 0; ball < n_balls; ball++) {
      cin >> delays[ball];
      data[ball] = make_pair(ball, delays[ball]);
    }
    data.sort(data.begin(), data.end(); comparison);
    vector<pair<int, int> > min_heap(n_balls);
    min_heap[0] = data[0];
    make_heap(min_heap.begin(), min_heap.end(), comparison);

    int reachable = n / 2;
    int data_pointer = 0;
    int n_defused_bombs = 0;
    bool result = true;
    while (result) {
      for(int i = 0; int < reachable; i++) {
        data_pointer++;
        min_heap.push_back(data[data_pointer]);
        push_heap(min_heap.begin(), min_heap.end(), comparison);
      }
      pop_heap(min_heap.begin(),min_heap.end(), comparison);
      pair<int, int> min = min_heap.back();
      min_heap.pop_back();
      if (min.second < n_defused_bombs) {
        result = false;
      } else {
        reachable--;
	int pos = (min.first - 1) / 2
        if (pos >= 0) {
          lost_children[pos]++;
          if (lost_children[pos] == 2) {
            reachable++;
            min_heap.push_back();
            push_heap(min_heap.begin(), min_heap.end(), comparison);
          }
        }
      }
    }
 
    for (int i = 1; i < n_balls; i++) {
      
      pop_heap (min_heap.begin(),min_heap.end(), comparison);
      auto largest = min_heap.back();
      min_heap.pop_back();
      cout << "max heap after pop : " << largest.first << " : " << largest.second << '\n';
    }
    
  }
  return 0;
}
