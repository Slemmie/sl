#pragma once

#include <vector>
#include <cassert>
#include <iostream>

//-//

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
		
		friend std::ostream& operator << (std::ostream& stream, const Edge& edge) {
			stream << "{[" << edge.from << " -> " << edge.to << "]: (" << edge.weight << ")}";
			return stream;
		}
		
	};
	
	std::vector <Edge> edges;
	std::vector <std::vector <int>> g;
	int size;
	
	Graph(int _size) :
	g(_size),
	size(_size)
	{ }
	
	virtual int add(int from, int to, A weight = 1) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		int ind = edges.size();
		g[from].push_back(ind);
		edges.push_back(Edge(from, to, weight));
		return ind;
	}
	
	virtual std::vector <int>& operator [] (int ind) {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
	virtual const std::vector <int>& operator [] (int ind) const {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Graph& graph) {
		stream << "[\n";
		for (int i = 0; i < graph.size; i++) {
			stream << "  " << i << ": {";
			for (int j = 0; j < (int)graph[i].size(); j++) {
				stream << "(" << graph.edges[graph[i][j]].to << ": ";
				stream << graph.edges[graph[i][j]].weight << ")";
				if (j + 1 != (int)graph[i].size()) {
					stream << ", ";
				}
			}
			stream << "}\n";
		}
		stream << "]";
		return stream;
	}
	
};
