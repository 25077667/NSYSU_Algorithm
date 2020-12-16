這是第一組的作業三
---

1. 使用算法：模擬退火

2. 程式碼要有註解：
想法相當 Naive, 這邊也沒有用到 C++ 高深的語法，以及複雜的運算，註解我們就盡量。

3. 如何編譯、執行、輸出結果、程式的執行時間及跑出的最佳解及路徑：
**請參照 Makefile**

### 編譯:
```    
g++ main.cpp -g -Ofast -Os -std=c++2a -o main.elf
```

### 執行:
```
cat ${TEST_FILE} | ./main.elf
```

### 輸出結果:
就 stdout 看你想怎麼導向

### 程式執行時間:
有一個指令叫做 `time` 請善用

### 跑出最佳解:
你給我無限的執行時間，我可以保證有最佳解。
如果沒有，就可能沒有最佳解。
就是機率問題而已：根據無限猴子定理(Infinite monkey theorem)，"The probability that an infinite randomly records of swapping cities will contain a best permutation of cities is 1."

退火就是找 city 起來交換，選比較好的存下來。
然而存在一機率函數 $p: t \to [0,1)$，在 $p(t)$ > `threshold` 的情況下，接保存交換結果。


給定城市數目 $N$，則且存在路徑最佳解 $\rho$
$\rho \in \Epsilon_{\cdots c_{i} \cdots}$ are best permutation of cities.

For $x$ is the number of cities, at the correct place.
Than, there are $N-x$ cities at the wrong place.

The probability function of swapping the wrong placed cities $f$ is $\frac(N-x){N}$.
Hence, for uniform random choosing, $\displaystyle{lim_{t\ to \infinity}} \frac{\partial f(x)}{\partial t} = 0$ 

翻成白話文來說，就是收斂速度會越來越慢。