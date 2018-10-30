// Copyright 2018 Stefan Adrian

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>

#include "./hashtable.h"
#include "./listgraph.h"
#include "./hashfunctions.h"

#define HMAX 10000

/*
		Functia citeste cele N orase din fisier si populeaza 
	cele doua hashtable-uri necesare in creearea unei instante 
	a clasei graf. Un hashtable asociaza fiecarui oras un 
	index(cuprins intre 0 si N-1), iar celalt asociaza 
	unui index(cuprins intre 0 si N-1) un oras.

*/

void readCities(unsigned int N, Hashtable<std::string, unsigned int>
&nameToindex, Hashtable<unsigned int, std::string> &indexToname)
{
	std::string city;
	int node = 0;

	for (unsigned int i = 0; i < N; ++i)
	{
		std::cin >> city;
		nameToindex.put(city, node);
		indexToname.put(node, city);
		++node;
	}
}

/*
		Functia citeste cele M autostrazi si creeaza 
	graful care va reprezenta harta rutiera.
*/

void readHighway(unsigned int M, ListGraph<std::string> &graph)
{
	std::string city1, city2;
	unsigned int i;

	for (i = 0; i < M; ++i)
	{
		std::cin >> city1 >> city2;
		graph.addArc(city1, city2);
	}
}

/*
		Functia citeste cele B orase in care se gaseste 
	o benzinarie, si marcheaza in vectorul stations 
	faptul ca orasul (identificat printr-un index de 
	la 0 la N-1) contine benzinarie.
*/

void readStations(std::vector<unsigned int> &stations,
ListGraph<std::string> &graph)
{
	std::string city;
	unsigned int B;

	std::cin >> B;

	for (unsigned int i = 0; i < B; ++i)
	{
		std::cin >> city;
		stations[graph.IndexValue(city)] = 1;
	}
}

/*
		Functia afiseaza orasele inaccesibile unui oras dat.
*/

void inaccessibleCities(ListGraph<std::string> &graph,
std::vector<unsigned int> &strongComp, unsigned int node, std::ofstream &task3)
{
	for (unsigned int i = 0; i < graph.getSize(); ++i)
	{
		if (strongComp[i] != strongComp[node])
		{
			task3 << graph.Value(i) << ' ';
		}
	}

	task3 << '\n';
}

/*
		Functia citeste cele K orase pentru care 
	se doreste lista cu orasele inaccesibile 
	si apeleaza functia de mai sus pentru a afisa 
	respectiva lista
*/

void readlistCity(ListGraph<std::string> &graph, std::vector<unsigned int>
&strongComp, unsigned int nr_comp)
{
	std::ofstream task3("task3.out");
	unsigned int K;
	std::string city;

	std::cin >> K;

	if (nr_comp == 1)
	{
		for (unsigned int i = 0; i < K; ++i)
		{
			std::cin >> city;
		}
		task3 << "Exista drum intre oricare doua orase\n";
		return;
	}

	for (unsigned int i = 0; i < K; ++i)
	{
		std::cin >> city;
		task3 << "Orase inaccesibile pentru " << city << ": ";
		inaccessibleCities(graph, strongComp, graph.IndexValue(city), task3);
	}

	task3.close();
}

/*
		Aceasta functie este folosita pentru a determina daca o masina aflata 
	intr-un oras poatea ajunge intr-un alt oras care contine benzinarie cu 
	combustibilul pe care-l are in rezervor.
*/

void bfs(unsigned int node, ListGraph<std::string> &graph,
std::vector<unsigned int> &stations, int X, int C, std::ofstream &task4)
{
	unsigned int n = graph.getSize();
	std::vector<unsigned int> neighbors;
	std::vector<int> gas(n, 0);

	std::queue<unsigned int> Queue;

	Queue.push(node);
	gas[node] = X;

	while (!Queue.empty())
	{
		node = Queue.front();
		Queue.pop();

		neighbors = graph.getNeighbors(node);

		if (gas[node] - C >= 0)
		{
			for (unsigned int i = 0; i < neighbors.size(); ++i)
			{
				if (gas[neighbors[i]] == 0)
				{
					if (stations[neighbors[i]] == 1)
					{
						task4 << "DA " << gas[node] - C << '\n';
						return;
					}

					Queue.push(neighbors[i]);
					gas[neighbors[i]] = gas[node] - C;
				}
			}
		} else {
			task4 << "NU\n";
			return;
		}
	}
}

/*
		Functia citeste cele L linii care contin descrierea unei masini 
	(orasul unde se gaseste, combustibilul din rezervor, consumul per-autostrada) 
	si apeleaza bfs de mai sus ca sa vada daca masina are sanse sau nu sa 
	ajunga intr-un oras cu benzinarie pentru a alimenta.
*/

void readCarDesc(ListGraph<std::string> &graph,
std::vector<unsigned int> &stations)
{
	int L, X, C;
	std::string city;
	std::ofstream task4("task4.out");

	std::cin >> L;

	for (int i = 0; i < L; ++i)
	{
		std::cin >> city;
		std::cin >> X >> C;
		bfs(graph.IndexValue(city), graph, stations, X, C, task4);
	}

	task4.close();
}

/*
		Functia afiseaza orasele interemdiare ale unei rute.
*/

void printRoute(int node, std::vector<int> &parents,
ListGraph<std::string> &graph, std::ofstream &task5)
{
	if (parents[node] != -1)
	{
		printRoute(parents[node], parents, graph, task5);
		task5 << graph.Value(node) << ' ';
	}
}

/*
	
		Functia sorteaza lexicografic orasele vecine ale unui oras.
*/

void sortCities(ListGraph<std::string> &graph,
std::vector<unsigned int> &neighbors)
{
	int ok, j = 0;
	std::string city1, city2;
	int n = (int)neighbors.size();

	do
	{
		ok = 1;

		for (int i = 0; i < n - 1 - j; ++i)
		{
			city1 = graph.Value(neighbors[i]);
			city2 = graph.Value(neighbors[i + 1]);

			if (city1 > city2)
			{
				neighbors[i] = neighbors[i] + neighbors[i + 1];
				neighbors[i + 1] = neighbors[i] - neighbors[i + 1];
				neighbors[i] = neighbors[i] - neighbors[i + 1];
				ok = 0;
			}
		}
		++j;
	} while (ok == 0);
}

/*
		Functia realizeaza o parcurgere bfs plecand dintr-un nod si 
	determina pentru fiecare nod parintele acestuia.
*/

void bfs(unsigned int source, unsigned int destination,
ListGraph<std::string> &graph, std::ofstream &task5)
{
	unsigned int n = graph.getSize(), ok = 0, node;
	std::vector<bool> visited(n, false);
	std::vector<int> parents(n, -1);
	std::vector<unsigned int> neighbors;

	std::queue<unsigned int> Queue;

	visited[source] = true;
	Queue.push(source);

	while (!Queue.empty())
	{
		node = Queue.front();
		Queue.pop();

		neighbors = graph.getNeighbors(node);
		sortCities(graph, neighbors);

		for (unsigned int i = 0; i < neighbors.size(); ++i)
		{
			if (visited[neighbors[i]] == false)
			{
				if (neighbors[i] == destination)
				{
					parents[neighbors[i]] = node;
					ok = 1;
					break;
				}

				parents[neighbors[i]] = node;
				visited[neighbors[i]] = true;
				Queue.push(neighbors[i]);
			}
		}

		if (ok == 1)
		{
			break;
		}
	}

	printRoute((int)parents[destination], parents, graph, task5);
}

/*
		Functia citeste cel P rute si detrmina ruta pe care 
	se gasesc cei mai multi calatori
*/

void readRoutes(Hashtable<route, unsigned int> &Routes,
ListGraph<std::string> &graph)
{
	unsigned int P, nr_pasagers, max;
	std::string source, destination;
	route R;
	std::ofstream task5("task5.out");

	std::cin >> P;

	std::cin >> source >> destination >> max;

	Routes.put(route(source, destination), max);

	for (unsigned int i = 1; i < P; ++i)
	{
		std::cin >> R.source >> R.destination >> nr_pasagers;

		nr_pasagers += Routes.getValue(R);

		Routes.put(R, nr_pasagers);

		if (nr_pasagers > max)
		{
			max = nr_pasagers;
			source = R.source;
			destination = R.destination;
		}
	}


	task5 << max << ' ' << source << ' ';
	bfs(graph.IndexValue(source), graph.IndexValue(destination), graph, task5);
	task5 << destination << '\n';

	task5.close();
}

/*
		Functia afiseaza in fisier orasul cu cel mai 
	mare grad de aglomeratie.
*/

void aggDegree(ListGraph<std::string> &graph)
{
	std::ofstream task1("task1.out");

	unsigned int max_inDegree = 0;
	std::string city = graph.maxInDegree(max_inDegree);

	task1 << city << ' ' << max_inDegree << '\n';

	task1.close();
}

/*
		Functia verifica daca harta este sa nu valida.
*/

void mapValidation(unsigned int nr_comp)
{
	std::ofstream task2("task2.out");

	if (nr_comp == 1)
	{
		task2 << "HARTA VALIDA\n";
	} else {
		task2 << "HARTA INVALIDA\n";
	}

	task2.close();
}

int main()
{
	unsigned int N, M, nr_comp;

	Hashtable<std::string, unsigned int> nameToindex(HMAX, &NameToIndex);
	Hashtable<unsigned int, std::string> indexToname(HMAX, &IndexToName);
	Hashtable<route, unsigned int> Routes(HMAX, hash);

	std::cin >> N >> M;

	std::vector<unsigned int> strongComp(N, 0);
	std::vector<unsigned int> stations(N, 0);

	readCities(N, nameToindex, indexToname);
	ListGraph<std::string> graph(N, nameToindex, indexToname);

	readHighway(M, graph);
	aggDegree(graph);

	nr_comp = graph.strongCompConex(strongComp);

	mapValidation(nr_comp);

	readStations(stations, graph);

	readlistCity(graph, strongComp, nr_comp);

	readCarDesc(graph, stations);

	readRoutes(Routes, graph);

	return 0;
}
