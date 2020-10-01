#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

inline double getDistance(pair<int, int> a, pair<int, int> b) {
  return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

double getPathLen(vector<pair<int, int>> cities) {
  double total = 0;
  pair<int, int> prev = make_pair(0, 0);
  for (auto i : cities) {
    if (i != *cities.begin())
      total += getDistance(prev, i);
    else
      prev = i; // The first time enter this loop
  }
  return total;
}

// Use pipeline to put in the stdin, plz
int main() {
  int num, x, y;
  vector<pair<int, int>> cities, cacheStatus;
  double minPath = numeric_limits<double>::max();
  while (cin >> num >> x >> y)
    cities.push_back(make_pair(x, y));

  sort(cities.begin(), cities.end());
  do {
    auto pathLen = getPathLen(cities);
    minPath = min(minPath, pathLen);
    if (minPath == pathLen)
      cacheStatus = cities;
  } while (next_permutation(cities.begin(), cities.end()));

  for (auto i : cacheStatus)
    cout << find(cities.begin(), cities.end(), i) - cities.begin() + 1 << " ";

  cout << endl << "Minimal length of path: " << minPath << endl;
  return 0;
}