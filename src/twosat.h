#pragma once

#include <vector>
#include <cassert>
#include <iostream>

//-//

struct Twosat {
	
	int size;
	std::vector <std::vector <int>> g;
	std::vector <int> ans, arrival, comp;
	
	Twosat(int _size) :
	size(_size),
	g(_size << 1)
	{ }
	
	inline void add(int from, int to) {
		assert(from >= 0 && from < (size << 1) && to >= 0 && to < (size << 1));
		g[from].push_back(to);
	}
	
	inline void reset() {
		m_time = -1;
		ans = std::vector <int> (size, -1);
		arrival = comp = std::vector <int> (size << 1, -1);
	}
	
	bool init() {
		reset();
		for (int i = 0; i < (size << 1); i++) {
			if (comp[i] == -1) {
				m_dfs(i);
			}
		}
		for (int i = 0; i < size; i++) {
			if (comp[i << 1] == comp[(i << 1) | 1]) {
				return false;
			}
		}
		return true;
	}
	
	int& operator [] (int ind) {
		assert(ind >= 0 && ind < size);
		return ans[ind];
	}
	
	int operator [] (int ind) const {
		assert(ind >= 0 && ind < size);
		return ans[ind];
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Twosat& ts) {
		stream << "{";
		for (int i = 0; i < ts.size; i++) {
			stream << ts[i];
			if (i + 1 != ts.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
private:
	
	int m_dfs(int node) {
		arrival[node] = ++m_time;
		int mn = arrival[node];
		m_stck.push_back(node);
		for (int x : g[node]) {
			if (comp[x] == -1) {
				mn = std::min(mn, arrival[x] == -1 ? m_dfs(x) : arrival[x]);
			}
		}
		if (arrival[node] == mn) {
			int v;
			do {
				v = m_stck.back();
				m_stck.pop_back();
				comp[v] = mn;
				if (ans[v >> 1] == -1) {
					ans[v >> 1] = 1 ^ (v & 1);
				}
			} while (node != v);
		}
		return arrival[node] = mn;
	}
	
	int m_time;
	std::vector <int> m_stck;
	
};
