#include "util.h"

void swap(int& a, int& b)
{
	a ^= b;
	b ^= a;
	a ^= b;
}

int greatestCommonFactorEuclid(int a, int b)
{
	if (a == b) {
		return a;
	}
	if (b > a) {
		swap(a, b);
	}
	if (b == 0) {
		return 0;
	}
	auto r = a % b;
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
	}
	return b;
}

int leastCommonMultipleEuclid(int a, int b) {
	return (a * b) / greatestCommonFactorEuclid(a, b);
}

int greatestCommonFactorNaive(int a, int b) {
	return (a * b) / leastCommonMultipleNaive(a, b);
}

int leastCommonMultipleNaive(int a, int b) {
	if (a == b) {
		return a;
	}
	auto aa = a;
	auto bb = b;
	while (aa != bb) {
		if (aa > bb) {
			bb += b;
		}
		else {
			aa += a;
		}
	}
	return aa;
}