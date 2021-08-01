#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <cassert>

//-//

template <typename A> class Sparse_table {
	
public:
	
	int size, width;
	std::vector <std::vector <A>> table;
	
	std::function <A(const A&, const A&)> merge;
	
	Sparse_table(const std::vector <A>& initial, const std::function <A(const A&, const A&)> _merge) :
	size(initial.size()),
	width(m_msb(initial.size())),
	merge(_merge)
	{
		assert(!initial.empty());
		table.resize(width);
		table[0] = initial;
		for (int b = 1; b < width; b++) {
			table[b].resize(size - (1 << b) + 1);
			for (int i = 0; i < size - (1 << b) + 1; i++) {
				table[b][i] = merge(table[b - 1][i], table[b - 1][i + (1 << (b - 1))]);
			}
		}
	}
	
	A get(int l, int r) const {
		assert(l <= r && l >= 0 && r < size);
		l++;
		int len = r - l + 1;
		if (!len) {
			return table[0][l - 1];
		}
		A ret = table[0][l - 1];
		for (int b = width - 1; b >= 0; b--) {
			if ((len >> b) & 1) {
				ret = merge(ret, table[b][l]);
				l += 1 << b;
			}
		}
		return ret;
	}
	
	friend std::ostream& operator << (std::ostream& stream, const Sparse_table& sparse_table) {
		for (int b = sparse_table.width - 1; b >= 0; b--) {
			stream << "[ {";
			for (int i = 0; i < (int)sparse_table.table[b].size(); i++) {
				stream << sparse_table.table[b][i];
				if (i + 1 != (int)sparse_table.table[b].size()) {
					stream << ", ";
				}
			}
			stream << "} ]";
			if (b) {
				stream << "\n";
			}
		}
		return stream;
	}
	
private:
	
	int m_msb(int x) {
		int r = 0;
		while (x) {
			r++;
			x >>= 1;
		}
		return r;
	}
	
};
