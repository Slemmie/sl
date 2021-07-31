#pragma once

#include <vector>
#include <numeric>
#include <iostream>

//-//

class Dsu {
	
public:
	
	std::vector <int> p;
	int size;
	
	Dsu(int _size) :
	p(_size),
	size(_size)
	{
		std::iota(p.begin(), p.end(), 0);
	}
	
	inline int get(int i) {
		return i == p[i] ? i : (p[i] = get(p[i]));
	}
	
	inline bool unite(int i, int j) {
		i = get(i);
		j = get(j);
		if (i != j) {
			p[i] = j;
			return true;
		}
		return false;
	}
	
	friend std::ostream& operator << (std::ostream& stream, Dsu& dsu) {
		stream << "{";
		for (int i = 0; i < dsu.size; i++) {
			stream << dsu.get(i);
			if (i + 1 != dsu.size) {
				stream << ", ";
			}
		}
		stream << "}";
		return stream;
	}
	
};
