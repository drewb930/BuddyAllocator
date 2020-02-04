FLAG= -Wall -std=c++11

all: memtest

Ackerman.o: Ackerman.cpp 
	g++ $(FLAG) -c -g Ackerman.cpp

BuddyAllocator.o : BuddyAllocator.cpp
	g++ $(FLAG) -c -g BuddyAllocator.cpp

Main.o : Main.cpp
	g++ $(FLAG) -c -g Main.cpp

memtest: Main.o Ackerman.o BuddyAllocator.o
	g++ $(FLAG) -o memtest Main.o Ackerman.o BuddyAllocator.o

clean:
	rm *.o

run:
	./memtest