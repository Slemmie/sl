#pragma once

#include <vector>
#include <iostream>

//-//

template <typename A> class Floyd_warshall {
	
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
	std::vector <std::vector <A>> g;
	std::vector <std::vector <bool>> bad;
	int size;
	A inf;
	
	Floyd_warshall(int _size, A _inf) :
	size(_size),
	inf(_inf)
	{ }
	
	void reset() {
		g = std::vector <std::vector <A>> (size, std::vector <A> (size, inf));
		for (int i = 0; i < size; i++) {
			g[i][i] = 0;
		}
		for (Edge& edge : edges) {
			g[edge.from][edge.to] = std::min(g[edge.from][edge.to], edge.weight);
		}
		bad = std::vector <std::vector <bool>> (size, std::vector <bool> (size, false));
	}
	
	inline void add(int from, int to, A weight = 1) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		edges.emplace_back(from, to, weight);
	}
	
	inline A dist(int from, int to) const {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		return bad[from][to] ? -inf : g[from][to];
	}
	
	inline bool has_path(int from, int to) const {
		return dist(from, to) < inf;
	}
	
	void init(bool has_negative = true) {
		reset();
		for (int k = 0; k < size; k++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (g[i][k] < inf && g[k][j] < inf) {
						g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);
					}
				}
			}
		}
		if (!has_negative) {
			return;
		}
		for (int k = 0; k < size; k++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (g[i][k] < inf && g[k][j] < inf && g[i][j] > g[i][k] + g[k][j]) {
						bad[i][j] = true;
					}
				}
			}
		}
		for (int k = 0; k < size; k++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if ((g[i][k] < inf && bad[k][j]) || (g[k][j] < inf && bad[i][k])) {
						bad[i][j] = true;
						g[i][j] = -inf;
					}
				}
			}
		}
	}
	
	std::vector <A>& operator [] (int ind) {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
	const std::vector <A>& operator [] (int ind) const {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Floyd_warshall& fw) {
		for (int i = 0; i < fw.size; i++) {
			stream << " [ {";
			for (int j = 0; j < fw.size; j++) {
				stream << fw[i][j];
				if (j + 1 != fw.size) {
					stream << ", ";
				}
			}
			stream << "} ]";
			if (i + 1 != fw.size) {
				stream << "\n";
			}
		}
		return stream;
	}
	
};
