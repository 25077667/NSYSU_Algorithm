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


// Return a pair<length, order of cities>
// @traversal: A map to keep the dp table, the key is the "viewed" cities as a 4
//             bytes(can contain 32 cities) `recordMask`. And the value is a
//             array of path length of each city would push_back to "viewed"
//             cities.
// @records: Record the current path of cities.
// @cities: A const vector to get cities info from user.
// @current: The current index of the city.
// @recordMask: A mask to recording "viewed" cities.
pair<double, vector<City>> dp(map<int, array<double, MAX_CITIES>> &traversal,
                              vector<City> records,
                              const vector<City> &cities,
                              int current,
                              int recordMask)
{
    // auto isRecorded = traversal.find(recordMask);
    // if (isRecorded != traversal.end() &&
    //     abs(isRecorded->second.at(current) -1) > 1e-7) {  // found record
    //     return make_pair(isRecorded->second.at(current), records);
    // } else {
        double minPathLen = numeric_limits<double>::infinity();
        int minMask = recordMask;
        vector<City> &minPath = records;
        for (int i = 0; i < cities.size(); i++) {
            if (((1 << i) & recordMask) && i != current) {
                auto [passPathLen, passCities] = dp(
                    traversal, minPath, cities, i, recordMask ^ (1 << current));
                if (passPathLen < minPathLen) {
                    minPathLen = passPathLen;
                    minPath = passCities;
                    minMask = recordMask ^ (1 << current);
                }
            }
        }

        auto mask = ((1 << current) | minMask);
        auto origBack = (minPath.size() ? minPath.back() : cities.at(current));
        minPath.push_back(cities.at(current));

        if(traversal.find(mask) == traversal.end())
            for (auto& j : traversal[mask])
                j = -1; 

        traversal[mask].at(current) =
            getDistance(origBack, cities.at(current)) + minPathLen;
        return make_pair(traversal[mask].at(current), minPath);
    //}
}

void output(const pair<double, vector<City>> result)
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
    cout << get<0>(result.second.at(0)) << " " << get<1>(result.second.at(0))
         << " " << get<2>(result.second.at(0)) << endl;

    cout << endl << "Minimal length of path: " << result.first << endl;
}

// Suppose starts at the first city
// Initialize `traversal` with cityes' distance
void init(map<int, array<double, MAX_CITIES>> &traversal,
          const vector<City> cities)
{
    for (int i = 0; i < cities.size(); i++) {
        for (auto& j : traversal[1 << i])
            j = -1;
        traversal[1 << i].at(i) = getDistance(cities.at(0), cities.at(i));
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
    cities.push_back(cities[0]);

    // DP table
    map<int, array<double, MAX_CITIES>> traversal;
    init(traversal, cities);

    auto result = dp(traversal, records, cities, 0, (1 << cities.size()) - 1);

    output(result);
    return 0;
}
