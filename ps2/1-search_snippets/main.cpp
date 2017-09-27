#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  int n_test_cases;
  int n_words;
  int pos;
  int n_positions = 0;
  map<int, int> query;
  cin >> n_test_cases;
  for (int test_case = 0; test_case < n_test_cases; test_case++) {
    cin >> n_words;
    vector<int> occurrences_per_word(n_words);
    for (int i = 0; i < n_words; i++) {
      cin >> occurrences_per_word[i];
      n_positions += occurrences_per_word[i];
    }

    for (int i = 0; i < n_words; i++) {
      for (int j = 0; j < occurrences_per_word[i]; j++) {
        cin >> pos;
        query[pos] = i;
      }
    }

    vector<int> positions(n_positions);
    int position_counter = 0;
    int pos_i = 0;
    for ( const auto &p : query ) {
      positions[pos_i] = p.first;
      pos_i++;
    }

    int word;
    int left_most_pointer = -1;
    int min_distance = n_positions;
    int distance;
    int words_found = 0;
    vector<int> word_selection(n_words, -1);
    for (int i = 0; i < n_positions; i++) {
      pos = positions[i];
      word = query[pos];
      if (word_selection[word] == -1) {
        words_found++;
      }
      if (word_selection[word] == left_most_pointer) {
        // TODO(kevinkle): make this more efficient
        int new_left = n_positions;
        word_selection[word] = i;
        for (int w = 0; w < n_words; w++) {
          if (word_selection[w] != -1) {
            new_left = min(new_left, word_selection[w]);
          }
        }
        left_most_pointer = new_left;
      } else {
        word_selection[word] = i;
      }
      if (words_found == n_words) {
        distance = pos - positions[left_most_pointer] + 1;
        cout << "left position: " << positions[left_most_pointer] << endl;
        cout << "right position: " << pos << endl;
        cout << "current distance: " << distance << endl;
        min_distance = min(min_distance, distance);
        cout << "min distance: " << min_distance << endl;
      }
    }
    while (left_most_pointer < n_positions) {
      word = query[positions[left_most_pointer]];
      word_selection[word] = -1;
      words_found--;
      left_most_pointer++;
      if (word_selection[query[positions[left_most_pointer]]] == -1) {
        cout << "found " << query[positions[left_most_pointer]] << " again at position " << positions[left_most_pointer] << endl;
        word_selection[query[positions[left_most_pointer]]] == left_most_pointer;
        words_found++;
      }
      if (words_found == n_words) {
        cout << "left position: " << positions[left_most_pointer] << endl;
        cout << "right position: " << positions[n_positions - 1] << endl;
        distance = n_positions - 1 - positions[left_most_pointer] + 1;
        cout << "current distance: " << distance << endl;
        min_distance = min(min_distance, distance);
        cout << "min distance: " << min_distance << endl;
      }
    }
    //min_distance++;
    cout << min_distance << endl;
  }
  return 0;
}
