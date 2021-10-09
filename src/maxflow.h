#pragma once

#include <sl/graph.h>

#include <cassert>
#include <vector>
#include <queue>

//-//

template <typename A> class Maxflow : public Graph <A> {
	
public:
	
	using Graph <A>::edges;
	using Graph <A>::g;
	using Graph <A>::size;
	
	Maxflow(int _size, A _inf) :
	Graph <A> (_size),
	m_inf(_inf),
	m_par(_size)
	{ }
	
	int add(int from, int to, A capacity) override {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		int ind = edges.size();
		g[from].push_back(ind);
		edges.emplace_back(from, to, capacity);
		g[to].push_back(ind + 1);
		edges.emplace_back(to, from, 0);
		return ind;
	}
	
	A init(int source, int sink) {
		A result = 0;
		while (m_bfs(source, sink)) {
			A flow = m_inf;
			for (int i = sink; i != source; i = edges[m_par[i]].from) {
				flow = std::min(flow, edges[m_par[i]].weight);
			}
			for (int i = sink; i != source; i = edges[m_par[i]].from) {
				edges[m_par[i]].weight -= flow;
				edges[m_par[i] ^ 1].weight += flow;
			}
			result += flow;
		}
		return result;
	}
	
private:
	
	A m_inf;
	
	std::vector <int> m_par;
	
	bool m_bfs(int source, int sink) {
		std::vector <char> vis(size, false);
		vis[source] = true;
		std::queue <int> q;
		q.push(source);
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			for (int x : g[v]) {
				auto& e = edges[x];
				if (vis[e.to] || e.weight <= 0) {
					continue;
				}
				m_par[e.to] = x;
				if (e.to == sink) {
					return true;
				}
				q.push(e.to);
				vis[e.to] = true;
			}
		}
		return false;
	}
	
};
