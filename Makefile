# Pill

all: pill

pill: pill.o jsoncpp.o
	g++ -Wall bin/pill.o bin/jsoncpp.o -o bin/pill
	rm -rf bin/*.o

pill.o: src/pill.cpp
	g++ -Wall -c src/pill.cpp -Ilib/jsoncpp -o bin/pill.o

loot.o: src/loot.cpp src/include/loot.h
	g++ -Wall -c src/loot.cpp src/include/loot.h -o bin/loot.o

# Libs

jsoncpp.o: lib/jsoncpp/jsoncpp.cpp
	g++ -Wall -c lib/jsoncpp/jsoncpp.cpp -Ilib/jsoncpp -o bin/jsoncpp.o

# System

clean:
	rm -rf bin/*

# Debug

debug: pill_debug jsoncpp_debug
	g++ -Wall -g bin/pill.o bin/jsoncpp.o -o bin/pill

pill_debug: src/pill.cpp
	g++ -Wall -g -c src/pill.cpp -Ilib/jsoncpp -o bin/pill.o

loot_debug: src/loot.cpp src/include/loot.h
	g++ -Wall -g -c src/loot.cpp src/include/loot.h -o bin/loot.o

jsoncpp_debug: lib/jsoncpp/jsoncpp.cpp
	g++ -Wall -g -c lib/jsoncpp/jsoncpp.cpp -Ilib/jsoncpp -o bin/jsoncpp.o
