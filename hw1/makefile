all: 
	g++ main.cpp -Os -O2 -std=c++14 -o main.elf -lpthread

plot: all
	cat "testFile/readfile.txt" | ./main.elf > output.txt 
	gnuplot plot.gp

clean:
	rm -f *.elf output*
