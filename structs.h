// Copyright 2018 Stefan Adrian

#ifndef STRUCTS_H_

#define STRUCTS_H_

#include <vector>
#include <string>

//  Aceasta structura descrie un element generic al hashtable-lului.

template <class Tkey, class Tvalue> struct element
{
	Tkey key;
	Tvalue value;

	element(Tkey key, Tvalue value)
	{
		this->key = key;
		this->value = value;
	}
};

//  Descrie un nod(identificat prin indexul asociat) al unui graf

struct Node
{
	unsigned int inDegree;
	unsigned int outDegree;
	std::vector<unsigned int> neighbors;

	Node()
	{
		inDegree = 0;
		outDegree = 0;
	}
};

//  Descrie o ruta

struct route
{
	std::string source;
	std::string destination;

	route()
	{
		source = "";
		destination = "";
	}

	route(std::string s, std::string d)
	{
		source = s;
		destination = d;
	}

	bool operator==(route R)
	{
		return (source == R.source) && (destination == R.destination);
	}
};

#endif  //  STRUCTS_H_
