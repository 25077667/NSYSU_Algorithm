#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <sstream>
#include <tuple>
using namespace std;
#define LIST_SIZE 8
#define MAX_ITER 10000

typedef tuple<int, int, int> City;


list<City> get_cities(ifstream &fs)
{
    string line;
    list<City> result;
    while (getline(fs, line)) {
        if (isdigit(line[0])) {
            stringstream ss;
            ss << line;
            int num, x, y;
            ss >> num >> x >> y;
            result.push_back(make_tuple(num, x, y));
        }
    }
    return result;
}

void print_cities(list<City> v)
{
    for (auto i : v)
        cout << get<0>(i) << " ";
    cout << endl;
}

void print_graph(list<City> v)
{
    for (auto i : v)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << "\n";
    cout << get<0>(v.front()) << " " << get<1>(v.front()) << " "
         << get<2>(v.front()) << "\n";
}

namespace detial
{
inline double get_distance(City a, City b) noexcept
{
    return sqrt(pow(get<1>(a) - get<1>(b), 2) + pow(get<2>(a) - get<2>(b), 2));
}

inline double get_path_len(const list<City> &cities)
{
    double total = 0;
    City prev;
    for (const auto &i : cities) {
        if (i != *cities.begin())  // Not the first time enter this loop
            total += get_distance(prev, i);
        prev = i;
    }
    // Back to the head point
    total += get_distance(prev, *cities.begin());
    return total;
}

// Split a list into 2 vectors with random cut point.
pair<list<City>, list<City>> split(list<City> cities)
{
    size_t split_size = cities.size() / 2;
    list<City> one(split_size), other(cities.size() - split_size);
    int cut_point = rand() % cities.size();

    // dup the orig list
    cities.insert(cities.end(), cities.begin(), cities.end());

    // [cut_point, cut_point + split_size)
    auto head = cities.begin();
    auto tail = cities.begin();
    advance(head, cut_point);
    advance(tail, cut_point + split_size);
    copy(head, tail, one.begin());

    // [cut_point + split_size, head + cities.size() / 2 - split_size)
    head = tail;
    tail = head;
    advance(tail, cities.size() / 2 - split_size);
    copy(head, tail, other.begin());
    return make_pair(one, other);
}

};  // namespace detial

list<City> greedy(list<City> cities)
{
    list<City> l(cities.begin(), cities.end());
    list<City> rec;
    rec.push_back(l.front());
    l.erase(l.begin());
    while (!l.empty()) {
        auto min_iter = min_element(
            l.begin(), l.end(),
            [&last = as_const(rec.back())](const City &c1, const City &c2) {
                return detial::get_distance(last, c1) <
                       detial::get_distance(last, c2);
            });

        rec.splice(rec.end(), l, min_iter);
    }
    return rec;
}

list<City> find_near(list<City> cities)
{
    // split into 2 list
    auto [one, another] = detial::split(cities);
    // greedy gen path
    auto left = greedy(one);
    auto right = greedy(another);

    // concate
    double tail_head = detial::get_distance(left.back(), right.front()) +
                       detial::get_distance(left.front(), right.back()),
           tail_tail = detial::get_distance(left.back(), right.back()) +
                       detial::get_distance(left.front(), right.front());

    if (tail_head < tail_tail)
        left.insert(left.end(), right.begin(), right.end());
    else
        left.insert(left.end(), right.rbegin(), right.rend());

    return left;
}

auto find_near_best(list<City> cities)
{
    list<City> _sol;
    double _sol_len = numeric_limits<double>::max();

    for (int i = 0; i < LIST_SIZE; i++) {
        list<City> tmp_sol = find_near(cities);
        auto tmp_sol_len = detial::get_path_len(tmp_sol);
        if (tmp_sol_len < _sol_len) {
            _sol_len = tmp_sol_len;
            _sol = move(tmp_sol);
        }
    }
    return make_pair(_sol_len, _sol);
}


auto tabu(list<City> cities)
{
    // The tabu list
    list<list<City>> l;

    // Init tabu list
    for (int i = 0; i < LIST_SIZE; i++) {
        auto [_sol_len, _sol] = find_near_best(cities);
        l.push_back(_sol);
    }

    // tabu search
    for (int i = 0; i < MAX_ITER; i++) {
        auto [_sol_len, _sol] = find_near_best(cities);
        // O(1), list size is LIST_SIZE
        if (find(l.begin(), l.end(), _sol) == l.end()) {
            l.erase(l.begin());  // erase the head
            l.push_back(_sol);
            cities = _sol;
        } else {
            i--;
        }
        // cerr << i << "\n";
    }

    return cities;
}

int main()
{
    ifstream ifs("eil51.tsp", ifstream::in);
    auto cities = get_cities(ifs);
    ifs.close();

    // run greedy first
    list<City> sol = greedy(cities); // 513.61
    sol = tabu(sol);

    #ifndef __DBG__
    #define OUTPUT(x) print_cities(x)
    #else
    #define OUTPUT(x) print_graph(x)
    #endif
    
    OUTPUT(sol);

    cout << detial::get_path_len(sol) << endl;

    return 0;
}