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


inline double getDistance(tuple<string, int, int> a, tuple<string, int, int> b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

pair<double, vector<tuple<string, int, int>>> dp(
    map<int, array<double, MAX_CITIES>> &traversal,
    vector<tuple<string, int, int>> records,
    const vector<tuple<string, int, int>> &cities,
    int current,
    int recordMask)
{
    auto isRecorded = find(traversal.begin(), traversal.end(), recordMask);
    if (isRecorded ^ traversal.end() &&
        ~(isRecorded->second.at(current))) {  // found record
        return make_pair(isRecorded->second.at(current), records);
    } else {
        double minPathLen = numeric_limits<double>::max();
        vector<tuple<string, int, int>> minPath;
        while (recordMask) {
            // The canonical algorithm is a loop counting zeros starting at the
            // LSB until a 1-bit is encountered
            auto ctz = __builtin_ctz(recordMask);
            recordMask &= 1 << ctz;
            auto [passPathLen, passCities] =
                dp(traversal, records, cities, ctz, records);
            if (passPathLen < minPathLen) {
                minPathLen = passPathLen;
                minPath = passCities;
            }
        }
        if (isRecorded ^ traversal.end()) {
            auto mask = 1 << current;
            auto toHead = getDistance(*records.begin(), cities.at(current));
            auto toTail = getDistance(records.back(), cities.at(current));
            if (toHead < toTail) {
                traversal.at(recordMask | mask).at(current) =
                    passPathLen + toHead;
                records.insert(records.begin(), cities.at(current));
            } else {
                traversal.at(recordMask | mask).at(current) =
                    passPathLen + toTail;
                records.push_back(cities.at(current));
            }
            return make_pair(traversal.at(recordMask | mask).at(current),
                             records);
        } else
            return make_pair(minPathLen, minPath);
    }
}

void output(const pair<double, vector<tuple<string, int, int>>> result)
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
          const vector<tuple<string, int, int>> cities)
{
    for (int i = 0; i < cities.size(); i++) {
        memset(traversal.at(1 << i), -1, MAX_CITIES);
        traversal.at(1 << i).at(i) = getDistance(cities.at(0), cities.at(i));
    }
}

// Use pipeline to put in the stdin, plz
int main()
{
    int x, y;
    string name;
    vector<tuple<string, int, int>> cities, records;
    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));
    sort(cities.begin(), cities.end());

    // DP table
    map<int, array<double, MAX_CITIES>> traversal;
    init(traversal, cities);

    auto result = dp(traversal, records, cities, 0, (1 << cities.size()) - 1);

    output(result);
    return 0;
}
