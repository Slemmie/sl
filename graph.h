#pragma once

template <typename A> class Graph {
	
public:
	
	struct Edge {
		
		int from, to;
		A weight;
		
		Edge(int _from = -1, int _to = -1, A _weight = 0) :
		from(_from),
		to(_to),
		weight(_weight)
		{ }
		
	};
	
	std::vector <Edge> edges;
	std::vector <std::vector <int>> g;
	int size;
	
	Graph(int _size) :
	g(_size),
	size(_size)
	{ }
	
	virtual int add(int from, int to, A weight) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		int ind = edges.size();
		g[from].push_back(ind);
		edges.push_back(Edge(from, to, weight));
		return ind;
	}
	
};
