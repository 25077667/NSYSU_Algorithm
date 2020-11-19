all: 
	g++ main.cpp -Os -Og -std=c++14 -o main.elf

plot: all
	cat "testFile/readfile.txt" | ./main.elf > output.txt 
	gnuplot plot.gp

clean:
	rm -f *.elf output*
