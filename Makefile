PROGRAM_NAME=testcase
OBJECTS=testcase.o base.o       

CXX=g++  #clang++


CXXFLAGS += -Wall -Werror
LIBS=-lm                  




$(PROGRAM_NAME):$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)

	@echo " "
	@echo "Compilation completed!"
	@echo " "



testcase.o:base.hpp testcase.cpp  
	$(CXX) $(CXXFLAGS) -c -o testcase.o testcase.cpp


base.o:base.hpp 
	$(CXX) $(CXXFLAGS) -c -o base.o base.cpp



clean:
	rm -f *.o
	rm -f *~
	rm -f core


cat: 
	cat Makefile


all:	$(PROGRAM_NAME) clean
