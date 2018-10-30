// Copyright 2018 Stefan Adrian

#ifndef HASHTABLE_H_

#define HASHTABLE_H_

#include <iostream>
#include <vector>

#include "./structs.h"

template <class Tkey, class Tvalue> class Hashtable
{
 private:
	unsigned int HMAX;
	unsigned int (*hash_function)(Tkey);
	std::vector<std::vector<element<Tkey, Tvalue> > > H;

 public:
	Hashtable()
	{
		HMAX = 0;
		hash_function = NULL;
	}

	Hashtable(unsigned int size, unsigned int (*h)(Tkey))
	{
		HMAX = size;
		hash_function = h;

		for (unsigned int i = 0; i < HMAX; ++i)
		{
			H.push_back(std::vector<element<Tkey, Tvalue> >());
		}
	}

	/*
		Adauga un element in hashtable
	*/

	void put(Tkey key, Tvalue value)
	{
		unsigned int index = hash_function(key) % HMAX;
		typename std::vector<element<Tkey, Tvalue> >::iterator it;

		for (it = H[index].begin(); it != H[index].end(); ++it)
		{
			if (it->key == key)
			{
				it->value = value;
				return;
			}
		}

		H[index].push_back(element<Tkey, Tvalue>(key, value));
	}

	/*
		Returneaza valoarea asociata cheii primite ca parametru
	*/

	Tvalue getValue(Tkey key)
	{
		unsigned int index = hash_function(key) % HMAX;
		typename std::vector<element<Tkey, Tvalue> >::iterator it;

		for (it = H[index].begin(); it != H[index].end(); ++it)
		{
			if (it->key == key)
			{
				return it->value;
			}
		}

		return Tvalue();
	}

	/*
		Returneaza numarul de bucket-uri
	*/

	unsigned int getSize()
	{
		return HMAX;
	}
};


#endif  //  HASHTABLE_H_
