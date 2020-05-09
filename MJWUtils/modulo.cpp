#include "util.h"

#include <vector>
#include <numeric>

int modularExponent(int base, int exponent, int modulus) {
	int result = 1;
	while (exponent--) {
		result *= base;
		result %= modulus;
	}
	return result;
}

int modularInverse(int value, int modulus)
{
	if (modulus == 1 || value == 0) {
		return 0;
	}
	int m = modulus;
	swap(value, m);

	auto inv = 1, invPrev = 0;
	while (m > 1) {
		auto d = div(value, m);
		value = m;
		m = d.rem;

		auto invNext = invPrev - d.quot * inv;
		invPrev = inv;
		inv = invNext;
	}

	if (inv < 0) {
		inv += modulus;
	}

    return inv;
}

int numBits(int v) {
	auto nbits = 0;
	while (v >>= 1) {
		nbits++;
	}
	return nbits;
}

int montgomeryReduction(int a, int b, int modulus) {
	// adapted from https://www.nayuki.io/page/montgomery-reduction-algorithm
	auto nbits = numBits(modulus);
	auto r = (int)pow(2, nbits);
	auto rinv = modularInverse(r, modulus);
	auto k = (r * rinv - 1) / modulus;
	auto abar = (a << nbits) % modulus; // also: (a * r) % modulus;
	auto bbar = (b << nbits) % modulus; // also: (b * r) % modulus;
	auto x = abar * bbar;
	auto s = (x * k) % r;
	auto t = x + (s * modulus);
	auto u = t >> nbits;
	auto cbar = (u < modulus) ? u : (u - modulus);
	auto c = (cbar * rinv) % modulus;
	return c;
}
