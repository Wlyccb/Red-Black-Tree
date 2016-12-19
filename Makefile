CXX=g++
CXXFlag=-g -Wall -c

bbst: RBTree.o main.o
	$(CXX) RBTree.o main.o -o bbst

main.o: main.cpp
	$(CXX) $(CXXFlag) main.cpp


RBTree.o: RBTree.cpp
	$(CXX) $(CXXFlag) RBTree.cpp
  
clean: 
	rm -f *.o
	rm bbst
