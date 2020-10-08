#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <thread>
#include <tuple>
#include <vector>

using namespace std;

inline double getDistance(tuple<int, int, int> a, tuple<int, int, int> b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

double getPathLen(vector<tuple<int, int, int>> cities)
{
    double total = 0;
    tuple<int, int, int> prev;
    for (auto i : cities) {
        if (i != *cities.begin())  // Not the first time enter this loop
            total += getDistance(prev, i);
        prev = i;
    }
    // Back to the start point
    total += getDistance(prev, *cities.begin());
    return total;
}

void bruteForce(vector<tuple<int, int, int>> cities,
                vector<pair<double, vector<tuple<int, int, int>>>> result)
{
    double minPath = numeric_limits<double>::max();
    vector<tuple<int, int, int>> cacheStatus;
    do {
        auto pathLen = getPathLen(cities);
        if (minPath > pathLen) {
            cacheStatus = cities;
            minPath = pathLen;
        }
    } while (next_permutation(cities.begin() + 2, cities.end()));

    result.at(get<0>(cities.at(1)) - 1) = make_pair(minPath, cacheStatus);
}

// Use pipeline to put in the stdin, plz
int main()
{
    int num, x, y;
    vector<tuple<int, int, int>> cities;
    vector<pair<double, vector<tuple<int, int, int>>>> result;
    vector<thread> threadPool;
    while (cin >> num >> x >> y)
        cities.push_back(make_tuple(num, x, y));

    sort(cities.begin(), cities.end());
    for (auto i = 1; i < cities.size(); i++) {
        iter_swap(cities.begin() + 1, cities.begin() + i);
        thread t(bruteForce, cities, move(result));
        iter_swap(cities.begin() + 1, cities.begin() + i);
        threadPool.push_back(t);
    }

    for (auto iter = threadPool.begin(); iter != threadPool.end(); iter++)
        (*iter).join();

    sort(result.begin(), result.end());
    for (auto i : result.at(0).second)
        cout << get<0>(i) << " ";

    cout << endl << "Minimal length of path: " << result.at(0).first << endl;
    return 0;
}