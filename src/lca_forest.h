#pragma once

#include <sl/dfs_forest.h>

//-//

template <typename A> class Lca_forest : public Dfs_forest <A> {
	
public:
	
	using Dfs_forest <A>::edges;
	using Dfs_forest <A>::g;
	using Dfs_forest <A>::size;
	using Dfs_forest <A>::par_node;
	using Dfs_forest <A>::pre;
	using Dfs_forest <A>::post;
	using Dfs_forest <A>::dep;
	using Dfs_forest <A>::dfs_all;
	
	int height;
	std::vector <std::vector <int>> bin;
	
	Lca_forest(int _size) :
	Dfs_forest <A> (_size),
	bin(_size)
	{ }
	
	void init() {
		if (par_node.empty()) {
			dfs_all();
		}
		int depth = 0;
		for (int i = 0; i < size; i++) {
			depth = std::max(depth, dep[i]);
		}
		height = 1;
		while ((1 << height) <= depth) {
			height++;
		}
		for (int i = 0; i < size; i++) {
			bin[i].resize(height, -1);
			bin[i][0] = par_node[i];
		}
		for (int b = 1; b < height; b++) {
			for (int i = 0; i < size; i++) {
				if (bin[i][b - 1] == -1) {
					continue;
				}
				bin[i][b] = bin[bin[i][b - 1]][b - 1];
			}
		}
	}
	
	inline bool is_anc(int ancest, int child) {
		return pre[ancest] <= pre[child] && post[ancest] >= post[child];
	}
	
	inline int up(int node, int steps) {
		if (steps >= (1 << height)) {
			return -1;
		}
		for (int b = height - 1; b >= 0; b--) {
			if  ((steps >> b) & 1) {
				if (node == -1) {
					break;
				}
				node = bin[node][b];
			}
		}
		return node;
	}
	
	inline int lca(int u, int v) {
		if (is_anc(u, v)) {
			return u;
		}
		if (is_anc(v, u)) {
			return v;
		}
		for (int b = height - 1; b >= 0; b--) {
			if (bin[u][b] == -1 || is_anc(bin[u][b], v)) {
				continue;
			}
			u = bin[u][b];
		}
		return bin[u][0];
	}
	
	inline A dist(int u, int v) {
		return dep[u] + dep[v] - dep[lca(u, v)] * 2;
	}
	
};
