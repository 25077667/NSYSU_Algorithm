#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <tuple>
#include <vector>

#define MAX_CITIES 16

using namespace std;
typedef tuple<string, int, int> City;


inline double getDistance(City a, City b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

pair<double, vector<City>> dp(map<int, array<double, MAX_CITIES>> &traversal,
                              vector<City> &&cities)
{
    vector<City> records;
    auto push2records = [&](int index) {
        records.push_back(cities[index]);
        cities.erase(cities.begin() + index);
    };
    push2records(0);
    int recordMask = 1;

    auto city2index = [&](City c) {
        return int(find(cities.begin(), cities.end(), c) - cities.begin());
    };
    auto backIndex = [&]() {
        return (!records.empty()) ? city2index(records.back()) : 0;
    };

    while (!cities.empty()) {
        auto bi = backIndex();
        // Turn off the last city in bit
        auto prevMin = min_element(traversal[recordMask & (~(1 << bi))].begin(),
                                   traversal[recordMask & (~(1 << bi))].end());

        // Get all length of current
        for (auto iter = cities.begin(); iter != cities.end(); iter++)
            traversal[recordMask][iter - cities.begin()] =
                *(prevMin) + getDistance(records.back(), *iter);

        auto currentMinIndex = min_element(traversal[recordMask].begin(),
                                           traversal[recordMask].end()) -
                               traversal[recordMask].begin();
        recordMask |= 1 << currentMinIndex;
        push2records(currentMinIndex);
    }
    return make_pair(*min_element(traversal[recordMask].begin(),
                                  traversal[recordMask].end()),
                     records);
}

void output(const pair<double, vector<City>> result)
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;

    cout << endl << "Minimal length of path: " << result.first << endl;
}

// Suppose starts at the first city
// Initialize `traversal` with cityes' distance
void init(map<int, array<double, MAX_CITIES>> &traversal,
          const vector<City> cities)
{
    // Init no any city is 0
    traversal[0] = {0};

    for (int i = 0; i < cities.size(); i++) {
        auto &distances = traversal[1 << i];
        for (int j = 0; j < cities.size(); j++)
            distances[j] = getDistance(cities[i], cities[j]);
    }
}

// Use pipeline to put in the stdin, plz
int main()
{
    int x, y;
    string name;
    vector<City> cities, records;
    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));
    sort(cities.begin(), cities.end());

    // DP table
    map<int, array<double, MAX_CITIES>> traversal;
    init(traversal, cities);

    auto result = dp(traversal, move(cities));

    output(result);
    return 0;
}
