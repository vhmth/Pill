all: pill

pill: pill.o
	g++ -Wall bin/pill.o -o bin/pill

pill.o: src/pill.cpp
	g++ -Wall -c src/pill.cpp -o bin/pill.o

clean:
	rm -rf bin/*.o
