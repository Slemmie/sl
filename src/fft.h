#pragma once

#include <vector>
#include <complex>

//-//

typedef std::complex <double> cpx;

class FFT {
	
public:
	
	template <typename A> static std::vector <A> mul_pol(const std::vector <A>& p1, const std::vector <A>& p2) {
		std::vector <std::vector <cpx>> p = {
			{ p1.begin(), p1.end() },
			{ p2.begin(), p2.end() }
		};
		int n = 1;
		while (n < int(p[0].size() + p[1].size())) {
			n <<= 1;
		}
		p[0].resize(n);
		p[0] = fft(p[0]);
		p[1].resize(n);
		p[1] = fft(p[1]);
		for (int i = 0; i < n; i++) {
			p[0][i] *= p[1][i];
		}
		p[0] = fft(p[0], true);
		std::vector <A> result(n);
		for (int i = 0; i < n; i++) {
			result[i] = round(p[0][i].real());
		}
		return std::vector <A> (result.begin(), result.begin() + p1.size() + p2.size() - 1);
	}
	
	static std::vector <cpx> fft(const std::vector <cpx>& p, double inverse = false) {
		int n = p.size();
		if (n == 1) {
			return std::vector <cpx> (p.begin(), p.end());
		}
		std::vector <std::vector <cpx>> q(2, std::vector <cpx> (n >> 1));
		for (int i = 0; i < (n >> 1); i++) {
			q[0][i] = p[i << 1];
			q[1][i] = p[(i << 1) + 1];
		}
		q[0] = fft(q[0], inverse);
		q[1] = fft(q[1], inverse);
		static const double s_pi = std::acos(-1);
		double angle = 2.0 * s_pi / n * (inverse ? -1 : 1);
		cpx w(1), wn(std::cos(angle), std::sin(angle));
		std::vector <cpx> result(n);
		for (int i = 0; i < (n >> 1); i++) {
			result[i] = q[0][i] + w * q[1][i];
			result[i + (n >> 1)] = q[0][i] - w * q[1][i];
			if (inverse) {
				result[i] /= 2;
				result[i + (n >> 1)] /= 2;
			}
			w *= wn;
		}
		return result;
	}
		
};
