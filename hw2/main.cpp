#include <algorithm>
#include <array>
#include <bitset>
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

vector<City> cities = {
    /*
     *
     *
     *
     *
     */
};

int city2index(City c)
{
    return int(find(cities.begin(), cities.end(), c) - cities.begin());
}


void printCities(vector<City> v)
{
    for (auto i : v)
        cout << city2index(i) << " ";
    cout << endl;
}

void printTraversal(map<int, pair<double, vector<City>>> traversal)
{
    for (auto i : traversal) {
        cout << "-------Mask: " << bitset<16>(i.first) << " -------\n";
        auto [j, k] = i.second;
        cout << j << ": ";
        for (auto l : k)
            cout << city2index(l) << " ";

        cout << endl;
        cout << "------------------------" << endl;
    }
}

inline double getDistance(City a, City b)
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

vector<vector<City>> mask2city(int mask)
{
    vector<vector<City>> result;
    vector<int> metaMask(cities.size(), 0);
    int num = 0;

    for (int i = 0; i < cities.size(); i++)
        metaMask[i] += (mask >> i) & 1;
    sort(metaMask.begin(), metaMask.end());
    do {
        vector<City> subCities;
        for (auto iter = metaMask.begin(); iter != metaMask.end(); iter++)
            if (*iter)
                subCities.push_back(cities[iter - metaMask.begin()]);
        result.push_back(subCities);
    } while (next_permutation(metaMask.begin(), metaMask.end()));
    return result;
}

int city2mask(vector<City> v)
{
    int mask = 0;
    for (auto _city : v)
        mask |= 1 << city2index(_city);
    return mask;
}

pair<double, vector<City>> dp(map<int, pair<double, vector<City>>> &traversal)
{
    int recordMask = 7;

    while (recordMask != (1 << (cities.size() + 1)) - 1) {
        auto city_permutation = mask2city(recordMask);

        for (auto i : city_permutation) {
            auto minPathLen = numeric_limits<double>::infinity();
            int minInsertPoint = 0;

            auto current = i.back();
            i.pop_back();

            auto [oldPathLen, oldPath] = traversal[city2mask(i)];
            // Process the ring
            for (int j = 0; j < i.size(); j++) {
                double cutLen =
                    getDistance(oldPath[j], oldPath[(j + 1) % i.size()]);
                double insertedLen =
                    getDistance(oldPath[j], current) +
                    getDistance(oldPath[(j + 1) % i.size()], current);
                auto currentLen = oldPathLen - cutLen + insertedLen;
                if (currentLen < minPathLen) {
                    minInsertPoint = (j + 1) % i.size();
                    minPathLen = currentLen;
                }
            }
            oldPath.insert(oldPath.begin() + minInsertPoint, current);

            // Put to traversal table
            traversal[city2mask(oldPath)] = make_pair(minPathLen, oldPath);
            // cout << "::: " << bitset<16>(city2mask(oldPath)) << endl;
        }
        // Update the recordMask
        recordMask = (recordMask << 1) | 1;
    }

    // Return
    return traversal[recordMask >> 1];
}

void output(const pair<double, vector<City>> result)
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;

    cout << get<0>(result.second[0]) << " " << get<1>(result.second[0]) << " "
         << get<2>(result.second[0]) << endl;
    cout << endl << "Minimal length of path: " << result.first << endl;
}

// Suppose starts at the first city
// Initialize `traversal` with cityes' distance
void init(map<int, pair<double, vector<City>>> &traversal)
{
    traversal[0] = make_pair(0, vector<City>());
    for (int i = 0; i < cities.size(); i++) {
        traversal[1 << i] = make_pair(0, vector<City>(1, cities[i]));
        for (int j = 0; j < cities.size(); j++) {
            if (i != j) {
                traversal[(1 << i) | (1 << j)] =
                    make_pair(getDistance(cities[i], cities[j]) * 2,
                              vector<City>({cities[i], cities[j]}));
            }
        }
    }
}

// Use pipeline to put in the stdin, plz
int main()
{
    int x, y;
    string name;

    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));
    sort(cities.begin(), cities.end());

    // DP table
    map<int, pair<double, vector<City>>> traversal;
    init(traversal);

    // printTraversal(traversal);

    auto result = dp(traversal);

    output(result);
    return 0;
}
