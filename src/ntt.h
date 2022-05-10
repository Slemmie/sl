#pragma once

#include <sl/mint.h>

#include <vector>

//-//

namespace NTT {
	
	template <int ROOT> void ntt(std::vector <mi>& input) {
		const int size = input.size();
		
		static std::vector <mi> roots(2, 1);
		for (static int i = 2, sh = 2; i < size; i <<= 1, sh++) {
			roots.resize(size);
			mi angle = pow(mi(ROOT), mod >> sh);
			for (int j = i; j < (i << 1); j++) {
				roots[j] = roots[j >> 1] * (j & 1 ? angle : mi(1));
			}
		}
		
		const int leading = 31 - __builtin_clz(size);
		
		std::vector <int> reverse(size, 0);
		for (int i = 0; i < size; i++) {
			reverse[i] = (reverse[i >> 1] | ((i & 1) << leading)) >> 1;
		}
		
		for (int i = 0; i < size; i++) {
			if (i < reverse[i]) {
				std::swap(input[i], input[reverse[i]]);
			}
		}
		
		for (int i = 1; i < size; i <<= 1) {
			for (int j = 0; j < size; j += i << 1) {
				for (int k = 0; k < i; k++) {
					mi delta = roots[i + k] * input[i + j + k];
					input[i + j + k] = input[j + k] - delta;
					input[j + k] += delta;
				}
			}
		}
	}

	template <int ROOT = 62> std::vector <mi> conv(std::vector <mi> u, std::vector <mi> v) {
		if (u.empty() || v.empty()) {
			return { };
		}
		
		const int size = 1 << (32 - __builtin_clz(u.size() + v.size() - 1));
		
		u.resize(size);
		v.resize(size);
		
		ntt <ROOT> (u);
		ntt <ROOT> (v);
		
		std::vector <mi> res(size);
		
		mi inv = mi(1) / size;
		for (int i = 0; i < size; i++) {
			res[-i & (size - 1)] = u[i] * v[i] * inv;
		}
		
		ntt <ROOT> (res);
		
		res.resize(u.size() + v.size() - 1);
		
		return res;
	}
	
}
