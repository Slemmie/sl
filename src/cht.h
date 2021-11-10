#pragma once

#include <set>

//-//

class CHT {
	
public:
	
	struct Eq {
		
		mutable long long k, m, p;
		
		bool operator < (const Eq& oth) const {
			return k < oth.k;
		}
		
		bool operator < (long long oth) const {
			return p < oth;
		}
		
	};
	
	std::multiset <Eq, std::less <>> st;
	
	void push(long long k, long long m) {
		auto it3 = st.insert({ k, m, 0 });
		auto it2 = it3++;
		auto it1 = it2;
		while (m_intersect(it2, it3)) {
			it3 = st.erase(it3);
		}
		if (it1 != st.begin() && m_intersect(--it1, it2)) {
			it2 = st.erase(it2);
			m_intersect(it1, it2);
		}
		it2 = it1;
		while (it2 != st.begin() && (--it1)->p >= it2->p) {
			it2 = st.erase(it2);
			m_intersect(it1, it2);
			it2 = it1;
		}
	}
	
	long long query(long long x) {
		auto it = st.lower_bound(x);
		return it->k * x + it->m;
	}
	
private:
	
	static inline const long long m_inf = 1LL << 60;
	
	long long m_fdiv(long long a, long long b) {
		return a / b - ((a ^ b) < 0 && a % b);
	}
	
	bool m_intersect(std::multiset <Eq, std::less <>>::iterator& a, std::multiset <Eq, std::less <>>::iterator& b) {
		if (b == st.end()) {
			a->p = m_inf;
			return false;
		}
		if (a->k == b->k) {
			a->p = a->m > b->m ? m_inf : -m_inf;
		} else {
			a->p = m_fdiv(b->m - a->m, a->k - b->k);
		}
		return a->p >= b->p;
	}
	
};
