#pragma once

#include <vector>
#include <functional>
#include <cassert>

//-//

class Centroid_dec {
	
public:
	
	int size;
	std::vector <std::vector <int>> g;
	std::vector <bool> vis;
	
	Centroid_dec(int _size) :
	size(_size),
	g(_size)
	{ }
	
	inline void add(int u, int v) {
		g[u].push_back(v);
		g[v].push_back(u);
	}
	
	inline void reset() {
		vis = std::vector <bool> (size, false);
		m_sub = std::vector <int> (size);
	}
	
	void solve(int node, const std::function <void(int)>& fun) {
		if (vis.empty()) {
			reset();
		}
		m_solve(fun, node);
	}
	
	void solve_all(const std::function <void(int)>& fun) {
		reset();
		for (int i = 0; i < size; i++) {
			if (!vis[i]) {
				m_solve(fun, i);
			}
		}
	}
	
	const std::vector <int>& operator [] (const int ind) const {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
	std::vector <int>& operator [] (const int ind) {
		assert(ind >= 0 && ind < size);
		return g[ind];
	}
	
private:
	
	void m_solve(const std::function <void(int)>& fun, int node) {
		int centroid = m_cen(m_size(node) >> 1, node);
		fun(centroid);
		vis[centroid] = true;
		for (int x : g[centroid]) {
			if (!vis[x]) {
				m_solve(fun, x);
			}
		}
	}
	
	int m_cen(int target_size, int node, int par = -1) {
		for (int x : g[node]) {
			if (!vis[x] && x != par && m_sub[x] >= target_size) {
				return m_cen(target_size, x, node);
			}
		}
		return node;
	}
	
	int m_size(int node, int par = -1) {
		m_sub[node] = 1;
		for (int x : g[node]) {
			if (!vis[x] && x != par) {
				m_sub[node] += m_size(x, node);
			}
		}
		return m_sub[node];
	}
	
	std::vector <int> m_sub;
	
};
