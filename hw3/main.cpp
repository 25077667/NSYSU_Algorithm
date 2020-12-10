#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

#define INIT_TEMP 100
#define END_TEMP 0.0001
#define LAMBDA 0.97
#define ITER_TIMES 512

typedef tuple<string, int, int> City;

void printCities(vector<City> v)
{
    for (auto i : v)
        cout << get<0>(i) << " ";
    cout << endl;
}

inline float getRand()
{
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double getDistance(City a, City b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}


double getPathLen(vector<City> cities)
{
    double total = 0;
    City prev;
    for (auto i : cities) {
        if (i != *cities.begin())  // Not the first time enter this loop
            total += getDistance(prev, i);
        prev = i;
    }
    // Back to the start point
    total += getDistance(prev, *cities.begin());
    return total;
}

void output(const pair<double, vector<City>> result)
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;

    cout << get<0>(result.second[0]) << " " << get<1>(result.second[0]) << " "
         << get<2>(result.second[0]) << endl;
    cout << endl << "Minimal length of path: " << result.first << endl;
}

static vector<City> _permutation(vector<City> currentState)
{
    iter_swap(currentState.begin() + int(getRand() * currentState.size()),
              currentState.begin() + int(getRand() * currentState.size()));
    // printCities(currentState);
    return currentState;
}

pair<double, vector<City>> sa(vector<City> &&cities)
{
    double minLen = numeric_limits<double>::infinity();
    for (double ct = INIT_TEMP; ct > END_TEMP; ct *= LAMBDA) {
        for (int i = 0; i < ITER_TIMES; i++) {
            auto newState = _permutation(cities);
            auto [newLen, oldLen] =
                make_pair(getPathLen(newState), getPathLen(cities));
            // printCities(newState);
            if (newLen < oldLen || getRand() < END_TEMP) {
                cities.swap(newState);
                minLen = newLen;
            }
        }
    }
    return make_pair(minLen, cities);
}

int main()
{
    vector<City> cities = {/**/};
    int x, y;
    string name;

    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));
    sort(cities.begin(), cities.end());

    auto result = sa(move(cities));
    output(result);

    return 0;
}