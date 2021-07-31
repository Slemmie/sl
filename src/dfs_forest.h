#pragma once

#include <sl/forest.h>

//-//

template <typename A> class Dfs_forest : public Forest <A> {
	
public:
	
	using Forest <A>::edges;
	using Forest <A>::g;
	using Forest <A>::size;
	
	std::vector <int> par_node, par_edge, pre, post, order, sub, root, dep;
	std::vector <A> dist;
	
	Dfs_forest(int _size, bool _init = true) :
	Forest <A> (_size)
	{ 
		if (_init) {
			init();
		}
	}
	
	void init() {
		par_node = par_edge = pre = post = std::vector <int> (size, -1);
		order.clear();
		sub = std::vector <int> (size, 0);
		root = dep = std::vector <int> (size, -1);
		dist = std::vector <A> (size);
	}
	
	void clear() {
		par_node.clear();
		par_edge.clear();
		pre.clear();
		post.clear();
		order.clear();
		sub.clear();
		root.clear();
		dep.clear();
		dist.clear();
	}
	
	void dfs(int node, bool reset_order = true) {
		if (par_node.empty()) {
			init();
		} else if (reset_order) {
			order.clear();
		}
		m_dfs_from(node);
	}
	
	void dfs_all() {
		init();
		for (int i = 0; i < size; i++) {
			if (dep[i] != -1) {
				continue;
			}
			m_dfs_from(i);
		}
		assert(size == (int)order.size());
	}
	
private:
	
	void m_dfs(int node) {
		pre[node] = order.size();
		order.push_back(node);
		sub[node] = 1;
		for (int x : g[node]) {
			if (x == par_edge[node]) {
				continue;
			}
			auto& e = edges[x];
			int v = e.from ^ e.to ^ node;
			dep[v] = dep[node] + 1;
			dist[v] = dist[node] + e.weight;
			par_node[v] = node;
			par_edge[v] = x;
			root[v] = root[node];
			m_dfs(v);
			sub[node] += sub[v];
		}
		post[node] = (int)order.size() - 1;
	}
	
	void m_dfs_from(int node) {
		m_prep_dfs(node);
		m_dfs(node);
	}
	
	void m_prep_dfs(int node) {
		dep[node] = 0;
		dist[node] = A { };
		root[node] = node;
		par_node[node] = par_edge[node] = -1;
	}
	
};
