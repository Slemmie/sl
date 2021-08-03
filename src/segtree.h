#pragma once

#include <vector>
#include <functional>

//-//

template <typename A, typename APPLY_ARG> class Segtree {
	
public:
	
	int size, n_size;
	std::vector <A> v;
	std::function <A(const A&, const A&)> merge;
	std::function <void(A&, const APPLY_ARG&)> apply;
	
	Segtree(int _size,
	const std::function <A(const A&, const A&)>& _merge,
	const std::function <void(A&, const APPLY_ARG&)>& _apply) :
	n_size(_size),
	merge(_merge),
	apply(_apply)
	{
		assert(_size > 0);
		size = 1;
		while (size < _size) {
			size <<= 1;
		}
		v.resize(size << 1, A { });
		make();
	}
	
	template <typename B> Segtree(const std::vector <B>& u,
	const std::function <A(const A&, const A&)>& _merge,
	const std::function <void(A&, const APPLY_ARG&)>& _apply) :
	n_size(u.size()),
	merge(_merge),
	apply(_apply)
	{
		assert(!u.empty());
		size = 1;
		while (size < (int)u.size()) {
			size <<= 1;
		}
		v.resize(size << 1, A { });
		make(u);
	}
	
	template <typename B> Segtree(int _size, const std::vector <B>& u,
	const std::function <A(const A&, const A&)>& _merge,
	const std::function <void(A&, const APPLY_ARG&)>& _apply) :
	n_size(_size),
	merge(_merge),
	apply(_apply)
	{
		assert(_size > 0);
		size = 1;
		while (size < _size) {
			size <<= 1;
		}
		v.resize(size << 1, A { });
		make(u);
	}
	
	void make() {
		m_make(0, 0, size);
	}
	
	template <typename B> void make(const std::vector <B>& u) {
		m_make(u, 0, 0, size);
	}
	
	void update(int ind, const APPLY_ARG& arg) {
		assert(ind >= 0 && ind < size);
		m_update(ind, 0, 0, size, arg);
	}
	
	A query(int l, int r) {
		assert(l >= 0 && l < r && r <= size);
		return m_query(l, r, 0, 0, size);
	}
	
	A query() {
		return query(0, n_size);
	}
	
	int find_first(int l, int r, const std::function <bool(const A&)>& check) {
		assert(l >= 0 && l < r && r <= size);
		return m_find_first(l, r, 0, 0, size, check);
	}
	
	int find_first(const std::function <bool(const A&)>& check) {
		return find_first(0, size, check);
	}
	
	int find_last(int l, int r, const std::function <bool(const A&)>& check) {
		assert(l >= 0 && l < r && r <= size);
		return m_find_last(l, r, 0, 0, size, check);
	}
	
	int find_last(const std::function <bool(const A&)>& check) {
		return find_last(0, size, check);
	}
	
private:
	
	int m_mid(int l, int r) {
		return (l + r) >> 1;
	}
	
	bool m_outside(int l, int r, int tl, int tr) {
		return l >= tr || r <= tl;
	}
	
	void m_make(int now, int l, int r) {
		if (!(r - l - 1)) {
			return;
		}
		int mid = m_mid(l, r);
		m_make((now << 1) + 1, l, mid);
		m_make((now << 1) + 2, mid, r);
		v[now] = merge(v[(now << 1) + 1], v[(now << 1) + 2]);
	}
	
	template <typename B> void m_make(const std::vector <B>& u, int now, int l, int r) {
		if (!(r - l - 1)) {
			if (l < (int)u.size()) {
				apply(v[now], APPLY_ARG(u[l]));
			}
			return;
		}
		int mid = m_mid(l, r);
		m_make(u, (now << 1) + 1, l, mid);
		m_make(u, (now << 1) + 2, mid, r);
		v[now] = merge(v[(now << 1) + 1], v[(now << 1) + 2]);
	}
	
	void m_update(int ind, int now, int l, int r, const APPLY_ARG& arg) {
		if (!(r - l - 1)) {
			apply(v[now], arg);
			return;
		}
		int mid = m_mid(l, r);
		if (ind < mid) {
			m_update(ind, (now << 1) + 1, l, mid, arg);
		} else {
			m_update(ind, (now << 1) + 2, mid, r, arg);
		}
		v[now] = merge(v[(now << 1) + 1], v[(now << 1) + 2]);
	}
	
	A m_query(int tl, int tr, int now, int l, int r) {
		if (l >= tl && r <= tr) {
			return v[now];
		}
		int mid = m_mid(l, r);
		A ret { };
		if (m_outside(l, mid, tl, tr)) {
			ret = m_query(tl, tr, (now << 1) + 2, mid, r);
		} else {
			if (m_outside(mid, r, tl, tr)) {
				ret = m_query(tl, tr, (now << 1) + 1, l, mid);
			} else {
				ret = merge(
				m_query(tl, tr, (now << 1) + 1, l, mid),
				m_query(tl, tr, (now << 1) + 2, mid, r));
			}
		}
		return ret;
	}
	
	int m_find_first_exists(int now, int l, int r,
	const std::function <bool(const A&)>& check) {
		if (!(r - l - 1)) {
			return l;
		}
		int mid = m_mid(l, r);
		if (check(v[(now << 1) + 1])) {
			return m_find_first_exists((now << 1) + 1, l, mid, check);
		}
		return m_find_first_exists((now << 1) + 2, mid, r, check);
	}
	
	int m_find_first(int tl, int tr, int now, int l, int r,
	const std::function <bool(const A&)>& check) {
		if (l >= tl && r <= tr) {
			if (!check(v[now])) {
				return -1;
			}
			return m_find_first_exists(now, l, r, check);
		}
		int mid = m_mid(l, r);
		int ret = -1;
		if (!m_outside(l, mid, tl, tr)) {
			ret = m_find_first(tl, tr, (now << 1) + 1, l, mid, check);
		}
		if (!m_outside(mid, r, tl, tr) && ret == -1) {
			ret = m_find_first(tl, tr, (now << 1) + 2, mid, r, check);
		}
		return ret;
	}
	
	int m_find_last_exists(int now, int l, int r,
	const std::function <bool(const A&)>& check) {
		if (!(r - l - 1)) {
			return l;
		}
		int mid = m_mid(l, r);
		if (check(v[(now << 1) + 2])) {
			return m_find_last_exists((now << 1) + 2, mid, r, check);
		}
		return m_find_last_exists((now << 1) + 1, l, mid, check);
	}
	
	int m_find_last(int tl, int tr, int now, int l, int r,
	const std::function <bool(const A&)>& check) {
		if (l >= tl && r <= tr) {
			if (!check(v[now])) {
				return -1;
			}
			return m_find_last_exists(now, l, r, check);
		}
		int mid = m_mid(l, r);
		int ret = -1;
		if (!m_outside(mid, r, tl, tr)) {
			ret = m_find_last(tl, tr, (now << 1) + 2, mid, r, check);
		}
		if (!m_outside(l, mid, tl, tr) && ret == -1) {
			ret = m_find_last(tl, tr, (now << 1) + 1, l, mid, check);
		}
		return ret;
	}
	
};
