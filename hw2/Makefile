all: 
	g++ main.cpp -g -Os -Ofast -std=c++2a -o main.elf

plot: all
	$(shell time -f '%es' -o exe_time bash -c 'cat testFile/readfile.txt | ./main.elf > output.txt')
	cat exe_time >> output.txt
	rm exe_time
	gnuplot plot.gp

dbg: all
	cat testFile/readfile.txt | ./main.elf

clean:
	rm -f *.elf output*
