CC   = g++
CFLAGS = -Wall -Ofast -Os -std=c++20

all: 
	$(CC) $(CFLAGS) main.cpp -o main.elf

plot: dbg
	$(shell time -f '%es' -o exe_time bash -c "./main.elf > out.txt")
	cat exe_time >> output.txt
	rm exe_time
	gnuplot plot.gp

dbg:
	$(CC) $(CFLAGS) -D __DBG__ main.cpp -o main.elf

clean:
	rm -f *.elf out* exe_time
