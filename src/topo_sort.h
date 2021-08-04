#pragma once

#include <vector>
#include <iostream>
#include <cassert>

//-//

class Topo_sort {
	
public:
	
	std::vector <std::vector <int>> g;
	std::vector <int> res, order;
	int size;
	
	Topo_sort(int _size) :
	g(_size),
	size(_size)
	{ }
	
	void add(int from, int to) {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		g[from].push_back(to);
	}
	
	void reset() {
		res.clear();
		order = std::vector <int> (size, -1);
		m_vis = std::vector <bool> (size, false);
	}
	
	void init() {
		reset();
		for (int i = 0; i < size; i++) {
			if (!m_vis[i]) {
				m_dfs(i);
			}
		}
	}
	
	int& operator [] (int ind) {
		assert(ind >= 0 && ind < size);
		return res[ind];
	}
	
	int operator [] (int ind) const {
		assert(ind >= 0 && ind < size);
		return res[ind];
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Topo_sort& g) {
		stream << "{";
		for (int i = 0; i < g.size; i++) {
			stream << g[i];
			if (i + 1 != g.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
private:
	
	void m_dfs(int node) {
		m_vis[node] = true;
		for (int x : g[node]) {
			if (!m_vis[x]) {
				m_dfs(x);
			}
		}
		order[node] = res.size();
		res.push_back(node);
	}
	
	std::vector <bool> m_vis;
	
};
