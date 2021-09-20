#pragma once

#include <sl/topo_sort.h>

//-//

class SCC {
	
public:
	
	std::vector <std::vector <int>> inverse;
	Topo_sort topo;
	std::vector <int> col, head;
	int size;
	int amount;
	
	SCC(int _size) :
	inverse(_size),
	topo(_size),
	size(_size),
	amount(0)
	{ }
	
	inline void add(int from, int to) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		inverse[to].push_back(from);
		topo.add(from, to);
	}
	
	inline int root(int col) {
		return head[col];
	}
	
	inline void reset() {
		col = std::vector <int> (size, -1);
		head.clear();
		amount = 0;
	}
	
	void init() {
		reset();
		topo.init();
		for (int i = size - 1; i >= 0; i--) {
			if (col[topo[i]] == -1) {
				head.push_back(topo[i]);
				m_dfs(topo[i], amount++);
			}
		}
	}
	
	int& operator [] (int ind) {
		assert(ind >= 0 && ind < size);
		return col[ind];
	}
	
	int operator [] (int ind) const {
		assert(ind >= 0 && ind < size);
		return col[ind];
	}
	
	friend std::ostream& operator << (std::ostream& stream, const SCC& scc) {
		stream << "{";
		for (int i = 0; i < scc.size; i++) {
			stream << scc[i];
			if (i + 1 != scc.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
private:
	
	void m_dfs(int node, int c) {
		col[node] = c;
		for (int x : inverse[node]) {
			if (col[x] == -1) {
				m_dfs(x, c);
			}
		}
	}
	
};
