// Copyright 2018 Stefan Adrian

#ifndef LISTGRAPH_H_

#define LISTGRAPH_H_

#include <vector>
#include <stack>

#include "./hashtable.h"
#include "./structs.h"

template <class T> class ListGraph
{
 private:
	unsigned int size;
	std::vector<Node> list;

	Hashtable<T, unsigned int> H1;
	Hashtable<unsigned int, T> H2;

 public:
	ListGraph(unsigned int size, Hashtable<T, unsigned int> &H1,
	Hashtable<unsigned int, T> &H2)
	{
		this->size = size;

		this->H1 = H1;
		this->H2 = H2;

		for (unsigned int i = 0; i < size; ++i)
		{
			list.push_back(Node());
		}
	}

	/*
		Adauga un arc intre cele doua noduri
	*/

	void addArc(T source, T destination)
	{
		unsigned int var1 = H1.getValue(source);
		unsigned int var2 = H1.getValue(destination);
		++list[var1].outDegree;
		++list[var2].inDegree;
		list[var1].neighbors.push_back(var2);
	}

	/*
		Returneaza un vector cu vecinii(identificati 
		prin indexul asociat) unui nod
	*/

	std::vector<unsigned int> getNeighbors(unsigned int node)
	{
		return list[node].neighbors;
	}

	/*
		Returneaza numarul de noduri al garfului
	*/

	unsigned int getSize()
	{
		return size;
	}

	/*
		Returneaza informatia din nodul care are
		gradul de intrare cel mai mare, si, de 
		asemenea, prin intermediul parametrului primit
		va retine valoarea acestui grad
	*/

	T maxInDegree(unsigned int &max)
	{
		max = list[0].inDegree;
		unsigned int var = 0;

		for (unsigned int i = 1; i < size; ++i)
		{
			if (list[i].inDegree > max)
			{
				max = list[i].inDegree;
				var = i;
			}
		}

		return H2.getValue(var);
	}

	/*
		Intoarce informatia din nodul care 
		are asociat indexul 'node'
	*/

	T Value(unsigned int node)
	{
		return H2.getValue(node);
	}

	/*
		Intoarce indexul asociat nodului care
		contine informatia 'node'
	*/

	unsigned int IndexValue(T node)
	{
		return H1.getValue(node);
	}

	/*
		Calculeaza graful tarnspus al grafului curent
	*/

	ListGraph<T> getTransGraph()
	{
		ListGraph<T> transGraph(size, H1, H2);
		T value;

		for (unsigned int i = 0; i < size; ++i)
		{
			value = H2.getValue(i);
			for (unsigned int j = 0; j < list[i].neighbors.size(); ++j)
			{
				transGraph.addArc(H2.getValue(list[i].neighbors[j]), value);
			}
		}

		return transGraph;
	}

	/*
		Determina o lista a nodurilor sortata descrescator dupa
	timpul de finalizare.
	*/

	void dfs(unsigned int node, std::vector<bool> &visited,
	std::stack<unsigned int> &Stack)
	{
		visited[node] = true;

		std::vector<unsigned int> neighbors = getNeighbors(node);

		for (unsigned int i = 0; i < neighbors.size(); ++i)
		{
			if (visited[neighbors[i]] == false)
			{
				dfs(neighbors[i], visited, Stack);
			}
		}

		Stack.push(node);
	}

	/*
		Realizeaza o parcurgere asupra grafului transpus
	    si identifica din ce componenta tare-conexa face 
	    parte fiecare nod
	*/

	void dfs(ListGraph<T> &transGraph, unsigned int node,
	std::vector<unsigned int> &strongComp, unsigned int comp)
	{
		strongComp[node] = comp;

		std::vector<unsigned int> neighbors = transGraph.getNeighbors(node);

		for (unsigned int i = 0; i < neighbors.size(); ++i)
		{
			if (strongComp[neighbors[i]] == 0)
			{
				dfs(transGraph, neighbors[i], strongComp, comp);
			}
		}
	}

	/*
		Intoarce numarul de componente tare-conexe, iar in
		vectorul primit ca parametru va retine pentru fiecare
		nod(identificat prin indexul asociat) componenta tare-
		conexa din care face parte
	*/

	unsigned int strongCompConex(std::vector<unsigned int> &strongComp)
	{
		std::vector<bool> visited(size, false);
		std::stack<unsigned int> Stack;
		unsigned int comp = 0, node;

		for (unsigned int i = 0; i < size; ++i)
		{
			if (visited[i] == false)
			{
				dfs(i, visited, Stack);
			}
		}

		ListGraph<T> transGraph = getTransGraph();

		while (!Stack.empty())
		{
			node = Stack.top();
			Stack.pop();
			if (strongComp[node] == 0)
			{
				dfs(transGraph, node, strongComp, ++comp);
			}
		}

		return comp;
	}
};

#endif  //  LISTGRAPH_H_
