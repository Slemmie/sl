#pragma once

#include <vector>
#include <cmath>

//-//

class Sieve {
	
public:
	
	static std::vector <int> get_primes(const int n) {
		if (n < 2) {
			return { };
		}
		std::vector <int> result = { 2 };
		const int BLOCK = 30000;
		int sqr = round(sqrt(n));
		std::vector <char> is_p(sqr + 1, true);
		std::vector <int> pr, start;
		for (int i = 3; i <= sqr; i += 2) {
			if (is_p[i]) {
				pr.push_back(i);
				start.push_back((i * i - 1) >> 1);
				for (int j = i * i; j <= sqr; j += i << 1) {
					is_p[j] = false;
				}
			}
		}
		std::vector <char> block(BLOCK);
		int right = (n - 1) >> 1;
		for (int left = 0; left < right; left += BLOCK) {
			std::fill(block.begin(), block.end(), true);
			for (size_t i = 0; i < pr.size(); i++) {
				int p = pr[i];
				int j = start[i];
				for (; j < BLOCK; j += p) {
					block[j] = false;
				}
				start[i] = j - BLOCK;
			}
			if (!left) {
				block[0] = false;
			}
			for (int i = 0; i < BLOCK && left + i <= right; i++) {
				if (block[i]) {
					result.push_back(((left + i) << 1) + 1);
				}
			}
		}
		return result;
	}
	
};
