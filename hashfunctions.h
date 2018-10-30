// Copyright 2018 Stefan Adrian

#ifndef HASHFUNCTIONS_H_

#define HASHFUNCTIONS_H_

#include <string>
#include "./structs.h"

unsigned int NameToIndex(std::string key)
{
	unsigned int hash = 5381;

	for (unsigned int i = 0 ; i < key.size(); ++i)
	{
		hash = ((hash << 5) + hash) + key[i];
	}

	return hash;
}

unsigned int IndexToName(unsigned int index)
{
	return index;
}

unsigned int hash(route R)
{
	return NameToIndex(R.source) + NameToIndex(R.destination);
}


#endif  //  HASHFUNCTIONS_H_
