CXX = g++
CXXFLAGS = -Wall -std=c++11

all: my_program

my_program: Application.o FinderLib.a 
	$(CXX) $(CXXFLAGS) -o my_program Application.o ./FinderLib.a

Application.o: Application.cpp 
	$(CXX) $(CXXFLAGS) -c Application.cpp

Finder.o: Finder.cpp 
	$(CXX) $(CXXFLAGS) -c Finder.cpp

FinderLib.a: Finder.o 
	ar rcs FinderLib.a Finder.o

clean: rm -f *.o *.a my_program