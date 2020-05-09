#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "types.h"
#include <vector>
#include <string>
#include <map>

// factors
void swap(int& a, int& b);

int greatestCommonFactorEuclid(int a, int b);
int leastCommonMultipleEuclid(int a, int b);
int greatestCommonFactorNaive(int a, int b);
int leastCommonMultipleNaive(int a, int b);

// modulo
int modularExponent(int base, int exponent, int modulus);
int modularInverse(int a, int m);
int montgomeryReduction(int a, int b, int modulus);

// geometry
Vector2 centerOfMass(std::vector<Vector2> points);
Vector2 interpolate(Vector2 start, Vector2 end, double percent);
Vector2 interpolate(std::vector<Vector2> points, double percent);
Vector2 bezier(std::vector<Vector2> points, double percent);
std::vector<Vector2> traceBezier(std::vector<Vector2> controlPoints, double resolution);

// conversions
std::vector<char> stringToCharVector(const std::string & s);
std::string charVectorToString(const std::vector<char> & bytes);
std::string charVectorToString(const std::vector<unsigned char>& bytes);
std::vector<char> bytesToHexstring(const std::vector<unsigned char>& bytes);
std::vector<unsigned char> hexstringToBytes(const std::vector<char> & hex);
std::vector<char> bytesToBase64(const std::vector<unsigned char> & bytes);
std::vector<unsigned char> base64ToBytes(const std::vector<char>& base64);

// crypto
std::vector<unsigned char> repeatingKeyXor(const std::vector<unsigned char> & a, const std::vector<unsigned char> & b);
std::vector<unsigned char> singleByteXor(const std::vector<unsigned char>& a, unsigned char b);
std::map<unsigned char, int> charHistogram(const std::vector<unsigned char> bytes);
double scoreCharHistogram(const std::map<unsigned char, int>& hist);
double determineSingleByteCipher(const std::vector<unsigned char>& bytes, unsigned char & cipher);
int hammingDistance(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b);
int estimateKeysize(const std::vector<unsigned char>& ciphertext);
