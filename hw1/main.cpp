#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>

using namespace std;

inline double getDistance(tuple<int, int, int> a, tuple<int, int, int> b) {
  return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

double getPathLen(vector<tuple<int, int, int>> cities) {
  double total = 0;
  tuple<int, int, int> prev;
  for (auto i : cities) {
    if (i != *cities.begin()) // Not the first time enter this loop
      total += getDistance(prev, i);
    prev = i;
  }
  // Back to the start point
  total += getDistance(prev, *cities.begin());
  return total;
}

// Use pipeline to put in the stdin, plz
int main() {
  int num, x, y;
  vector<tuple<int, int, int>> cities, cacheStatus;
  double minPath = numeric_limits<double>::max();
  while (cin >> num >> x >> y)
    cities.push_back(make_tuple(num, x, y));

  sort(cities.begin(), cities.end());
  do {
    auto pathLen = getPathLen(cities);
    if (minPath > pathLen) {
      cacheStatus = cities;
      minPath = pathLen;
    }
  } while (next_permutation(cities.begin(), cities.end()));

  for (auto i : cacheStatus)
    cout << get<0>(i) << " ";

  cout << endl << "Minimal length of path: " << minPath << endl;
  return 0;
}