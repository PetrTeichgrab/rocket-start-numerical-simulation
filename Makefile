
all:
	g++ -g -O2 -o sim main.cpp -lsimlib -lm

run: all
	./sim
	python visualization.py