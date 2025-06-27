CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

main.o: main.cpp utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

process.o: process.cpp process.h
	$(CXX) $(CXXFLAGS) -c process.cpp -o process.o

scheduler.o: scheduler.cpp scheduler.h
	$(CXX) $(CXXFLAGS) -c scheduler.cpp -o scheduler.o


all: scheduler.o process.o main.o utils.o
	$(CXX) $(CXXFLAGS) process.o main.o utils.o scheduler.o -o cfs_scheduler

clean:
	rm -f *.o cfs_scheduler 