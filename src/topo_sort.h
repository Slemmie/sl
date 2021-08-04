#pragma once

#include <sl/graph.h>

//-//

template <typename A = int> class Topo_sort : public Graph <A> {
	
public:
	
	using Graph <A>::edges;
	using Graph <A>::g;
	using Graph <A>::size;
	
	std::vector <int> res, order;
	
	Topo_sort(int _size) :
	Graph <A> (_size)
	{ }
	
	void reset() {
		res.clear();
		order = std::vector <int> (size, -1);
	}
	
	void init() {
		reset();
		for (int i = 0; i < size; i++) {
			m_dfs(i);
		}
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Topo_sort& g) {
		stream << "{";
		for (int i = 0; i < g.size; i++) {
			stream << g.res[i];
			if (i + 1 != g.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
private:
	
	void m_dfs(int node) {
		if (order[node] != -1) {
			return;
		}
		for (int x : g[node]) {
			m_dfs(edges[x].to);
		}
		order[node] = res.size();
		res.push_back(node);
	}
	
};
