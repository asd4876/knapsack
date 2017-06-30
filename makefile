knapsack: main.o knapsack.o
	g++ -o knapsack main.o knapsack.o
main.o: main.cpp knapsack.h
	g++ -Wall -fexceptions -g -std=c++11 -c main.cpp -o main.o
knapsack.o: knapsack.h knapsack.cpp
	g++ -Wall -fexceptions -g -std=c++11 -c knapsack.cpp -o knapsack.o

clean:
	rm -rf knapsack *.o
