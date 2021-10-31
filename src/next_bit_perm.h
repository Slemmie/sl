#pragma once

#include <limits>

//-//

template <typename A> inline A next_bit_perm(A x) {
	if (x <= 0) {
		return 0;
	}
	A tmp = x | (x - 1);
	if (tmp == std::numeric_limits <A>::max()) {
		return x >> (__builtin_ffs(x) - 1);
	}
	return (tmp + 1) | ((~tmp & -(~tmp)) - 1) >> (__builtin_ffs(x));
}
