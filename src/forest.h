#pragma once

#include <sl/graph.h>

//-//

template <typename A> class Forest : public Graph <A> {
	
public:
	
	using Graph <A>::edges;
	using Graph <A>::g;
	using Graph <A>::size;
	
	Forest(int _size) :
	Graph <A> (_size)
	{ }
	
	int add(int from, int to, A weight = 1) override {
		assert(from >= 0 && from < size && to >= 0 && to < size);
		int ind = edges.size();
		assert(ind < size - 1);
		g[from].push_back(ind);
		g[to].push_back(ind);
		edges.emplace_back(from, to, weight);
		return ind;
	}
	
};
