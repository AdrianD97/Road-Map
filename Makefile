build: roadMap

tema2: roadMap.cpp hashtable.h hashfunctions.h listgraph.h structs.h
	g++ -Wall --std=c++11 $^ -o roadMap

clean: 
	rm roadMap
