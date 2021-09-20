#pragma once

#include <vector>
#include <functional>

//-//

template <typename node> class Lazy_segtree {
	
public:
	
	int size, n_size;
	std::vector <node> v;
	
	Lazy_segtree(int _size) :
	n_size(_size)
	{
		assert(_size > 0);
		size = 1;
		while (size < _size) {
			size <<= 1;
		}
		v.resize(size << 1, node { });
		make();
	}
	
	template <typename A> Lazy_segtree(const std::vector <A>& _v) :
	n_size(_v.size())
	{
		assert(!_v.empty());
		size = 1;
		while (size < (int)_v.size()) {
			size <<= 1;
		}
		v.resize(size << 1, node { });
		make(_v);
	}
	
	template <typename A> Lazy_segtree(int _size, const std::vector <A>& _v) :
	n_size(_size)
	{
		assert(_size > 0);
		size = 1;
		while (size < _size) {
			size <<= 1;
		}
		v.resize(size << 1, node { });
		make(_v);
	}
	
	void make() {
		m_make(0, 0, size);
	}
	
	template <typename A> void make(const std::vector <A>& _v) {
		m_make(_v, 0, 0, size);
	}
	
	template <typename... A> void update(int l, int r, const A&... arg) {
		assert(l >= 0 && l < r && r <= size);
		m_update(l, r, 0, 0, size, arg...);
	}
	
	template <typename... A> void update(const A&... arg) {
		update(0, n_size, arg...);
	}
	
	node query(int l, int r) {
		assert(l >= 0 && l < r && r <= size);
		return m_query(l, r, 0, 0, size);
	}
	
	node query() {
		return query(0, n_size);
	}
	
	int find_first(int l, int r, const std::function <bool(const node&)>& check) {
		assert(l >= 0 && l < r && r <= size);
		return m_find_first(l, r, 0, 0, size, check);
	}
	
	int find_first(const std::function <bool(const node&)>& check) {
		return find_first(0, n_size, check);
	}
	
	int find_last(int l, int r, const std::function <bool(const node&)>& check) {
		assert(l >= 0 && l < r && r <= size);
		return m_find_lsat(l, r, 0, 0, size, check);
	}
	
	int find_last(const std::function <bool(const node&)>& check) {
		return find_last(0, n_size, check);
	}
	
private:
	
	inline int m_mid(int l, int r) {
		return (l + r) >> 1;
	}
	
	inline bool m_outside(int l, int r, int tl, int tr) {
		return l >= tr || r <= tl;
	}
	
	inline node* m_child(int ind, int l, int r) {
		return r - l - 1 ? &v[ind] : nullptr;
	}
	
	inline void m_push(int now, int l, int r) {
		v[now].push(m_child((now << 1) + 1, l, r), m_child((now << 1) + 2, l, r), l, r);
	}
	
	void m_make(int now, int l, int r) {
		if (!(r - l - 1)) {
			return;
		}
		int mid = m_mid(l, r);
		m_make((now << 1) + 1, l, mid);
		m_make((now << 1) + 2, mid, r);
		v[now] = node::merge(v[(now << 1) + 1], v[(now << 1) + 2]);
	}
	
	template <typename A> void m_make(const std::vector <A>& _v, int now, int l, int r) {
		if (!(r - l - 1)) {
			if (l < (int)_v.size()) {
				v[now].apply(_v[l], l, r);
				m_push(now, l, r);
			}
			return;
		}
		int mid = m_mid(l, r);
		m_make(_v, (now << 1) + 1, l, mid);
		m_make(_v, (now << 1) + 2, mid, r);
		v[now] = node::merge(v[(now << 1) + 1], v[(now << 1) + 2]);
		
	}
	
	template <typename... A> void m_update(int tl, int tr, int now, int l, int r, const A&... arg) {
		m_push(now, l, r);
		if (l >= tr || r <= tl) {
			return;
		}
		if (l >= tl && r <= tr) {
			v[now].apply(arg..., l, r);
			m_push(now, l, r);
			return;
		}
		int mid = m_mid(l, r);
		m_update(tl, tr, (now << 1) + 1, l, mid, arg...);
		m_update(tl, tr, (now << 1) + 2, mid, r, arg...);
		v[now] = node::merge(v[(now << 1) + 1], v[(now << 1) + 2]);
	}
	
	node m_query(int tl, int tr, int now, int l, int r) {
		m_push(now, l, r);
		if (l >= tl && r <= tr) {
			return v[now];
		}
		int mid = m_mid(l, r);
		node ret = node { };
		if (m_outside(l, mid, tl, tr)) {
			ret = m_query(tl, tr, (now << 1) + 2, mid, r);
		} else {
			if (m_outside(mid, r, tl, tr)) {
				ret = m_query(tl, tr, (now << 1) + 1, l, mid);
			} else {
				ret = node::merge(
				m_query(tl, tr, (now << 1) + 1, l, mid),
				m_query(tl, tr, (now << 1) + 2, mid, r));
			}
		}
		return ret;
	}
	
	int m_find_first_exists(int now, int l, int r,
	const std::function <bool(const node&)>& check) {
		m_push(now, l, r);
		if (!(r - l - 1)) {
			return l;
		}
		int mid = m_mid(l, r);
		m_push((now << 1), l, mid);
		if (check(v[(now << 1) + 1])) {
			return m_find_first_exists((now << 1) + 1, l, mid, check);
		}
		return m_find_first_exists((now << 1) + 2, mid, r, check);
	}
	
	int m_find_first(int tl, int tr, int now, int l, int r,
	const std::function <bool(const node&)>& check) {
		m_push(now, l, r);
		if (l >= tl && r <= tr) {
			if (!check(v[now])) {
				return -1;
			}
			return m_find_first_exists(now, l, r, check);
		}
		int mid = m_mid(l, r);
		if (!m_outside(l, mid, tl, tr)) {
			return m_find_first(tl, tr, (now << 1) + 1, l, mid, check);
		}
		if (!m_outside(mid, r, tl, tr)) {
			return m_find_first(tl, tr, (now << 1) + 2, mid, r, check);
		}
		return -1;
	}
	
	int m_find_last_exists(int now, int l, int r,
	const std::function <bool(const node&)>& check) {
		v[now].push(m_child((now << 1) + 1, l, r), m_child((now << 1) + 2, l, r), l, r);
		if (!(r - l - 1)) {
			return l;
		}
		int mid = m_mid(l, r);
		m_push((now << 1) + 2, mid, r);
		if (check(v[(now << 1) +2 ])) {
			return m_find_last_exists((now << 1) + 2, mid, r, check);
		}
		return m_find_last_exists((now << 1) + 1, l, mid, check);
	}
	
	int m_find_last(int tl, int tr, int now, int l, int r,
	const std::function <bool(const node&)>& check) {
		m_push(now, l, r);
		if (l >= tl && r <= tr) {
			if (!check(v[now])) {
				return -1;
			}
			return m_find_last_exists(now, l, r, check);
		}
		int mid = m_mid(l, r);
		if (!m_outside(mid, r, tl, tr)) {
			return m_find_last(tl, tr, (now << 1) + 2, mid, r, check);
		}
		if (!m_outside(l, mid, tl, tr)) {
			return m_find_last(tl, tr, (now << 1) + 1, l, mid, check);
		}
		return -1;
	}
	
};
