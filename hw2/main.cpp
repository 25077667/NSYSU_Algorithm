#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <tuple>
#include <vector>

using namespace std;


// result format: (length, path)
// @length: A double of path length
// @path: A vector of cities, the order of vector is the order of minimal cities
// path
pair<double, vector<tuple<string, int, int>>> result;
// DP table
map<int, double> traversal;

inline double getDistance(tuple<string, int, int> a, tuple<string, int, int> b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

// Update the global variable result
double dp(const vector<tuple<string, int, int>> &cities,
          int current,
          int recorded)
{
    auto isRecorded = traversal.find(recorded);
    if (isRecorded == traversal.end()) {  // not found
        int mask = 1 << current;
        int minIndex = -1;
        traversal[recorded | mask] = numeric_limits<double>::max();
        for (int i = 0; i < cities.size(); i++) {
            if (i != current && recorded & (1 << i)) {
                auto fromDP = dp(cities, i, recorded & (~mask)) +
                              getDistance(cities.at(i), cities.at(current));
                if (fromDP < traversal[recorded | mask]) {
                    minIndex = i;
                    traversal[recorded | mask] = fromDP;
                }
            }
        }
        result.second.push_back(cities.at(minIndex));
        return traversal[recorded | current];
    } else {
        return isRecorded->second;
    }
}

void output()
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
    cout << get<0>(result.second.at(0)) << " " << get<1>(result.second.at(0))
         << " " << get<2>(result.second.at(0)) << endl;

    cout << endl << "Minimal length of path: " << result.first << endl;
}

void init(int numCities)
{
    for (int i = 0; i < numCities; i++)
        traversal[1 << i] = 0;
}

// Use pipeline to put in the stdin, plz
int main()
{
    int x, y;
    string name;
    vector<tuple<string, int, int>> cities;
    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));

    init(cities.size());

    result.first = dp(cities, 0, (1 << cities.size()) - 1);

    output();
    return 0;
}
