TEST_FILE=testFile/berlin52.tsp

all: 
	g++ main.cpp -g -Ofast -Os -std=c++2a -o main.elf -fsanitize=address

plot: all
	$(shell time -f '%es' -o exe_time bash -c "cat ${TEST_FILE} | ./main.elf > output.txt")
	cat exe_time >> output.txt
	rm exe_time
	gnuplot plot.gp

dbg: all
	cat ${TEST_FILE} | ./main.elf

clean:
	rm -f *.elf output*
