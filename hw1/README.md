# 簡介
這份作業叫我們使用「窮舉法」(也就是暴力法啦)

所以我們這樣寫(使用 permutation)。


# 用法

編譯：
```
make
```

執行（CLI）：
```
make
./main.elf
```
or
```
make
time cat "readfile.txt" | ./main.elf > output.txt
```

繪圖：
```
make plot
```
it can meet all the requirements of homework

清理：
```
make clean
```

# 環境

必須是支援完整 `POSIX` 的環境

我們使用 pthread API 已提高效能

建議使用 `Ubuntu 18.04` 以上

另外 g++ 必須支援 C++14 （Supported from GNU GCC 4.9）

建議使用 `GCC 6.1` 以上（因為 GCC 6.1 以上預設編譯標準為 C++14）

## TL;DR

Less requirement:
```
Ubuntu 18.04
GCC 6.1
```
