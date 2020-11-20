all: 
	g++ main.cpp -Os -Og -std=c++2a -o main.elf -lpthread

plot: all
	$(shell time -f '%es' -o exe_time bash -c 'cat testFile/readfile.txt | ./main.elf > output.txt')
	cat exe_time >> output.txt
	gnuplot plot.gp

clean:
	rm -f *.elf output* exe_time
