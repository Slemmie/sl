#pragma once

#include <functional>

#include <sl/mint.h>

//-//

template <typename A, typename type> class Rolling_hash {
	
public:
	
	int size;
	A data;
	std::function <type(const type&)> convert;
	mi base;
	std::vector <mi> pref, suff;
	
	Rolling_hash(const A& _data, const std::function <type(const type&)>& _convert, mi _base) :
	size(_data.size()),
	data(_data),
	convert(_convert),
	base(_base)
	{
		m_pow(size);
		m_inv(size);
		pref.resize(size);
		suff.resize(size);
		int ind = 0;
		for (auto it = data.begin(); it != data.end(); it++) {
			pref[ind] = (ind ? pref[ind - 1] : mi(0)) + mi(convert(*it)) * m_pow(ind);
			ind++;
		}
		ind = size - 1;
		for (auto it = data.rbegin(); it != data.rend(); it++) {
			suff[ind] = (ind + 1 < size ? suff[ind + 1] : mi(0)) + mi(convert(*it)) * m_pow(size - ind - 1);
			ind--;
		}
	}
	
	inline mi hash(int l, int r) {
		assert(l >= 0 && l <= r && r < size);
		return m_inv(l) * (pref[r] - (l ? pref[l - 1] : mi(0)));
	}
	
	inline mi hash_suf(int l, int r) {
		assert(l >= 0 && l <= r && r < size);
		return m_inv(size - r - 1) * (suff[l] - (r + 1 < size ? suff[r + 1] : mi(0)));
	}
	
	inline bool match(int l1, int r1, int l2, int r2) {
		assert(l1 >= 0 && l1 <= r1 && r1 < size);
		assert(l2 >= 0 && l2 <= r2 && r2 < size);
		return hash(l1, r1) == hash(l2, r2);
	}
	
	inline bool match_prefix(int l1, int r1, int l2, int r2) {
		assert(l1 >= 0 && l1 <= r1 && r1 < size);
		assert(l2 >= 0 && l2 <= r2 && r2 < size);
		int len = std::min(r1 - l1, r2 - l2);
		return match(l1, l1 + len, l2, l2 + len);
	}
	
	inline bool match_suffix(int l1, int r1, int l2, int r2) {
		assert(l1 >= 0 && l1 <= r1 && r1 < size);
		assert(l2 >= 0 && l2 <= r2 && r2 < size);
		int len = std::min(r1 - l1, r2 - l2);
		return match(r1 - len, r1, r2 - len, r2);
	}
	
	inline int lc_prefix(int l1, int r1, int l2, int r2) {
		assert(l1 >= 0 && l1 <= r1 && r1 < size);
		assert(l2 >= 0 && l2 <= r2 && r2 < size);
		int l = 1, r = std::min(r1 - l1, r2 - l2), res = 0;
		while (l <= r) {
			int mid = (l + r) >> 1;
			if (match(l1, l1 + mid, l2, l2 + mid)) {
				res = mid;
				l = mid + 1;
			} else {
				r = mid - 1;
			}
		}
		return res;
	}
	
	inline int lv_suffix(int l1, int r1, int l2, int r2) {
		assert(l1 >= 0 && l1 <= r1 && r1 < size);
		assert(l2 >= 0 && l2 <= r2 && r2 < size);
		int l = 1, r = std::min(r1 - l1, r2 - l2), res = 0;
		while (l <= r) {
			int mid = (l + r) >> 1;
			if (match(r1 - mid, r1, r2 - mid, r2)) {
				res = mid;
				l = mid + 1;
			} else {
				r = mid - 1;
			}
		}
		return res;
	}
	
	inline bool is_palindrome(int l, int r) {
		assert(l >= 0 && l <= r && r < size);
		return hash(l, r) == hash_suf(l, r);
	}
	
private:
	
	mi m_pow(int ind) {
		if (ind < 0) {
			return 1;
		}
		static int ready = 0;
		static std::vector <mi> res = { 1 };
		if (ind <= ready) {
			return res[ind];
		}
		int make_ready = ind + (ind >> 4);
		res.resize(make_ready + 1);
		static mi cur_base = 1;
		for (int i = ready + 1; i <= make_ready; i++) {
			cur_base *= base;
			res[i] = cur_base;
		}
		ready = make_ready;
		return res[ind];
	}
	
	mi m_inv(int ind) {
		if (ind < 0) {
			return 1;
		}
		static int ready = 0;
		static std::vector <mi> res = { 1 };
		if (ind <= ready) {
			return res[ind];
		}
		int make_ready = ind + (ind >> 4);
		res.resize(make_ready + 1);
		for (int i = ready + 1; i <= make_ready; i++) {
			res[i] = mi(1) / m_pow(i);
		}
		ready = make_ready;
		return res[ind];
	}
	
};
