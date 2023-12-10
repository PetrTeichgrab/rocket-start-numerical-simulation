
all:
	g++ -g -O2 -o sim main.cpp -lsimlib -lm

2d:
	g++ -g -O2 -o sim2d sim2D.cpp -lsimlib -lm
	./sim2d

run: all
	./sim
	python visualization.py