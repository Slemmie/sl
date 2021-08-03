#pragma once

#include <vector>
#include <iostream>

//-//

template <typename A> class Bellman_ford {
	
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
	std::vector <A> g;
	std::vector <bool> bad;
	int size, start;
	A inf;
	
	Bellman_ford(int _size, A _inf) :
	size(_size),
	start(-1),
	inf(_inf)
	{ }
	
	void reset(int _start) {
		assert(_start >= 0 && _start < size);
		start = _start;
		g = std::vector <A> (size, inf);
		bad = std::vector <bool> (size, false);
		g[start] = 0;
	}
	
	inline void add(int from, int to, A weight = 1) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		edges.emplace_back(from, to, weight);
	}
	
	inline A dist(int to) const {
		assert(to >= 0 && to < size);
		return bad[to] ? -inf : g[to];
	}
	
	inline bool has_path(int to) const {
		return g[to] < inf;
	}
	
	void init(int _start, bool has_negative = true) {
		reset(_start);
		for (int i = 0; i < size; i++) {
			for (Edge& edge : edges) {
				if (g[edge.from] < inf) {
					g[edge.to] = std::min(g[edge.to], g[edge.from] + edge.weight);
				}
			}
		}
		if (!has_negative) {
			return;
		}
		for (Edge& edge : edges) {
			if (g[edge.from] < inf && g[edge.to] > g[edge.from] + edge.weight) {
				bad[edge.to] = true;
			}
		}
		for (int i = 0; i < size; i++) {
			for (Edge& edge : edges) {
				bad[edge.to] = bad[edge.to] | bad[edge.from];
			}
		}
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Bellman_ford& bf) {
		stream << "{";
		for (int i = 0; i < bf.size; i++) {
			stream << bf.dist(i);
			if (i + 1 != bf.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
};
