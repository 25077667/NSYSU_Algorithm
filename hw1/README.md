# 簡介
這份作業叫我們使用「窮舉法」(也就是暴力法啦)

所以這樣寫(使用 permutation)。

# 用法
> TODO: 會再補 makefile，必須使用 GNU/Linux 環境

目前:

```
g++ -O2 -g -std=c++11 main.cpp -o main.elf
cat readfile.txt | main.elf
```


如果你需要執行時間：

```
time cat readfile.txt | main.elf
```