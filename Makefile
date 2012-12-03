# Pill

all: pill

pill: pill.o scout.o grunt.o jsoncpp.o
	$(CXX) -Wall bin/pill.o bin/scout.o bin/grunt.o bin/jsoncpp.o -o bin/pill
	rm -rf bin/*.o

pill.o: src/pill.cpp
	$(CXX) -Wall -c src/pill.cpp -Isrc/include -o bin/pill.o

scout.o: src/scout.cpp src/include/scout.h
	$(CXX) -Wall -c src/scout.cpp -Ilib/jsoncpp -o bin/scout.o

grunt.o: src/grunt.cpp src/include/grunt.h
	$(CXX) -Wall -c src/grunt.cpp -o bin/grunt.o

loot.o: src/loot.cpp src/include/loot.h
	$(CXX) -Wall -c src/loot.cpp -o bin/loot.o

# Debug

debug: pill_debug scout_debug grunt_debug jsoncpp_debug
	$(CXX) -Wall -g bin/pill.o bin/scout.o bin/grunt.o bin/jsoncpp.o -o bin/pill

pill_debug: src/pill.cpp
	$(CXX) -Wall -g -c src/pill.cpp -Isrc/include -o bin/pill.o

scout_debug: src/scout.cpp src/include/scout.h
	$(CXX) -Wall -g -c src/scout.cpp -Ilib/jsoncpp -o bin/scout.o

grunt_debug: src/grunt.cpp src/include/grunt.h
	$(CXX) -Wall -g -c src/grunt.cpp -o bin/grunt.o

loot_debug: src/loot.cpp src/include/loot.h
	$(CXX) -Wall -g -c src/loot.cpp -o bin/loot.o

# Libs

jsoncpp.o: lib/jsoncpp/jsoncpp.cpp
	$(CXX) -Wall -c lib/jsoncpp/jsoncpp.cpp -Ilib/jsoncpp -o bin/jsoncpp.o

jsoncpp_debug: lib/jsoncpp/jsoncpp.cpp
	$(CXX) -Wall -g -c lib/jsoncpp/jsoncpp.cpp -Ilib/jsoncpp -o bin/jsoncpp.o

# System

clean:
	rm -rf bin/*
