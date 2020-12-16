#include <sys/sysinfo.h>  // nproc
#include <algorithm>
#include <cmath>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

#define INIT_TEMP 1000
#define END_TEMP 0.0001
#define LAMBDA 0.97
#define ITER_TIMES 512
#define SWAP_THRESHOLD (1 / INIT_TEMP)

typedef tuple<string, int, int> City;

// 這是 debug 用的東東，可以拿掉，在 -Ofast 參數底下，compiler 會幫我拿掉
void printCities(vector<City> v)
{
    for (auto i : v)
        cout << get<0>(i) << " ";
    cout << endl;
}

inline float getRand()
{
    // C++11 的 random_device
    // C++ 為了解決通常大家都用時間當作亂數種子，但是時間並非隨機
    // 因此於 C++11 提供了一個讓我們產生接近隨機的亂數種子
    // 讓生成的亂數無法被預測。
    random_device rd;
    // 一個偽隨機亂數產生器，中文叫做「梅森旋轉演算法」
    // https://zh.wikipedia.org/zh-tw/%E6%A2%85%E6%A3%AE%E6%97%8B%E8%BD%AC%E7%AE%97%E6%B3%95
    // Mersenne Twister會在 [0,2^{k}-1] 的區間之間生成離散型均勻分布的隨機數。
    mt19937 generator(rd());
    // 均勻實數分佈，資料型態是 float, 取樣區間是 [0, 1)
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

// 純粹輸出沒什麼好解釋的，看不懂的話就去讀一下 std::tuple<T...> 怎麼取值
void output(const pair<double, vector<City>> result)
{
    for (auto i : result.second)
        cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;

    cout << get<0>(result.second[0]) << " " << get<1>(result.second[0]) << " "
         << get<2>(result.second[0]) << endl;
    cout << endl << "Minimal length of path: " << result.first << endl;
}

// 函式名 sa 就是 sa
pair<double, vector<City>> sa(vector<City> cities)
{
    // Uniform 選出兩個 city 交換
    // 他是一個 lambda function(From C++11)
    // 為了可讀性使用的方式, 相比兩次 iter_swap，像能相同
    // 因為這邊的 return 是 xvalue，會隱式呼叫 move operator
    // 所以總計 Copy 次數為 1
    // --------
    // 相比兩次 iter_swap 也是會有一次 Copy 狀態到 `newState`，寫法如下
    // ```
    // auto [c1, c2] = make_pair(int(getRand() * currentState.size(),
    //                           int(getRand() * currentState.size());
    //
    // iter_swap(currentState.begin() + c1, currentState.begin() + c2);
    // auto newState = currentState;    // Copy 發生於此
    // iter_swap(currentState.begin() + c1, currentState.begin() + c2);
    // ```
    // 如此，有沒有覺得我用 lambda 的方式比較優雅呢？
    auto _permutation = [](vector<City> currentState) {
        iter_swap(currentState.begin() + int(getRand() * currentState.size()),
                  currentState.begin() + int(getRand() * currentState.size()));
        return currentState;
    };

    // RAII
    double minLen = numeric_limits<double>::infinity();
    // 生於憂患，死於安樂
    // 初始溫度 INIT_TEMP, 熱寂溫度 END_TEMP, 溫度依比率遞減
    for (double ct = INIT_TEMP; ct > END_TEMP; ct *= LAMBDA) {
        for (int i = 0; i < ITER_TIMES; i++) {
            auto newState = _permutation(cities);
            // C++17 的 structured binding, 酷吧！
            auto [newLen, oldLen] =
                make_pair(getPathLen(newState), getPathLen(cities));
            // 距離比較小嗎？ 或是機率跳過去？
            if (newLen < oldLen || getRand() < SWAP_THRESHOLD * ct) {
                // C++ vector 的 swap 比較省效能
                // 詳情請參考 move semantic
                // 這 Python 就做不到，因為 Python list 是 mutable,
                // 你只能複製一遍寫進去蓋掉原值，沒辦法作到 move 操作
                // 這效能衝擊是很大的，有 N 個 city，效能就差 N/8 倍
                // 偉哉我大 C++
                cities.swap(newState);
                minLen = newLen;
            }
        }
    }
    // 回傳一個 pair，前面是路徑長，後面是排列
    return make_pair(minLen, cities);
}

int main()
{
    vector<City> cities = {/**/};
    int x, y;
    string name;

    while (cin >> name >> x >> y)
        cities.push_back(make_tuple(name, x, y));
    // 有排序比較安心，其實也可以不排（在hw1爆搜的 next_permutation 會用到）
    sort(cities.begin(), cities.end());

    // 相同時間只有一個執行緒覺得有點浪費，所以多開幾個
    vector<pair<double, vector<City>>> ansPool;
    vector<future<pair<double, vector<City>>>> threadPool(get_nprocs());

    // 開起來去背景跑
    for (auto &i : threadPool)
        i = async(sa, cities);

    // 回收執行緒的結果
    for (auto iter = threadPool.begin(); iter != threadPool.end(); iter++)
        ansPool.push_back((*iter).get());

    // 選出最好的，這邊可以用 min 在 O(N) 的情況下選出最好，但是因為這邊 log(N)
    // 幾乎是常數 所以 sort 也不會慢到哪裡去，而且這樣程式也比較簡潔
    sort(ansPool.begin(), ansPool.end());
    // STL sort 預設是升冪排序，所以取第一個就可以了
    output(ansPool[0]);

    return 0;
}
