#include "util.h"

#include <algorithm>
#include <map>

std::vector<unsigned char> repeatingKeyXor(const std::vector<unsigned char> & a, const std::vector<unsigned char> & key) {
	std::vector<unsigned char> xbytes(a.size());

	for (int i = 0; i < a.size(); i++) {
		xbytes[i] = a[i] ^ key[i % key.size()];
	}

	return xbytes;
}

std::vector<unsigned char> singleByteXor(const std::vector<unsigned char> & a, unsigned char b) {
    std::vector<unsigned char> x(a.size());
    for (int i = 0; i < x.size(); i++) {
        x[i] = a[i] ^ b;
    }
	return x;
}

std::map<unsigned char, int> charHistogram(const std::vector<unsigned char> bytes) {
    std::map<unsigned char, int> hist;
    for (auto& b : bytes) {
        if (!hist.contains(b)) {
            hist[b] = 0;
        }
        hist[b]++;
    }
    return hist;
}

std::map<char, double> charfreq_english = {
    {'a', 8.167},
    {'b', 1.492},
    {'c', 2.782},
    {'d', 4.253},
    {'e', 12.70},
    {'f', 2.228},
    {'g', 2.015},
    {'h', 6.094},
    {'i', 6.966},
    {'j', 0.153},
    {'k', 0.772},
    {'l', 4.025},
    {'m', 2.406},
    {'n', 6.749},
    {'o', 7.507},
    {'p', 1.929},
    {'q', 0.095},
    {'r', 5.987},
    {'s', 6.327},
    {'t', 9.056},
    {'u', 2.758},
    {'v', 0.978},
    {'w', 2.360},
    {'x', 0.150},
    {'y', 1.974},
    {'z', 0.074},
    {'A', 8.167},
    {'B', 1.492},
    {'C', 2.782},
    {'D', 4.253},
    {'E', 12.70},
    {'F', 2.228},
    {'G', 2.015},
    {'H', 6.094},
    {'I', 6.966},
    {'J', 0.153},
    {'K', 0.772},
    {'L', 4.025},
    {'M', 2.406},
    {'N', 6.749},
    {'O', 7.507},
    {'P', 1.929},
    {'Q', 0.095},
    {'R', 5.987},
    {'S', 6.327},
    {'T', 9.056},
    {'U', 2.758},
    {'V', 0.978},
    {'W', 2.360},
    {'X', 0.150},
    {'Y', 1.974},
    {'Z', 0.074},
    {' ', 6.5}
};

double scoreCharHistogram(const std::map<unsigned char, int> & hist) {
    double score = 0;
    for (auto& c : hist) {
        if (charfreq_english.contains(c.first)) {
            score += charfreq_english[c.first] * c.second;
        }
        else {
            //score += (c.second * -2);
        }
    }
    return score;
}

double determineSingleByteCipher(const std::vector<unsigned char>& bytes, unsigned char& cipher) {
    std::vector<double> scores(256);
    for (int i = 0; i <= 255; i++) {
        auto decrypted = singleByteXor(bytes, (unsigned char)i);
        auto hist = charHistogram(decrypted);
        scores[i] = scoreCharHistogram(hist);
    }

    cipher = (unsigned char)std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));
    return scores[cipher];
}

int countBits(unsigned char b) {
    int c = 0;
    while (b) {
        c += (b & 1);
        b >>= 1;
    }
    return c;
}

int hammingDistance(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b) {
    int d = 0;
    int i = 0;
    for (i = 0; i < std::min(a.size(), b.size()); i++) {
        d += countBits(a[i] ^ b[i]);
    }
    for (; i < std::max(a.size(), b.size()); i++) {
        d += (a.size() > b.size())
            ? countBits(a[i])
            : countBits(b[i]);
    }
    return d;
}

int estimateKeysize(const std::vector<unsigned char>& ciphertext) {
    int bestKeyscore = INT_MAX;
    int bestKeysize = 0;
    for (int ks = 3; ks <= 40; ks++) {
        int naverages = ciphertext.size() / ks;
        double thisKeyscore = 0;
        std::vector<unsigned char> spanA;
        std::vector<unsigned char> spanB(ciphertext.begin(), ciphertext.begin() + ks);
        for (int a = 1; a < naverages; a++) {
            spanA = spanB;
            spanB = std::vector<unsigned char>(ciphertext.begin() + (a * ks), ciphertext.begin() + ((a + 1) * ks));
            thisKeyscore += hammingDistance(spanA, spanB);
        }
        thisKeyscore = thisKeyscore / (naverages * ks);
        if (thisKeyscore < bestKeyscore) {
            bestKeyscore = thisKeyscore;
            bestKeysize = ks;
        }
    }
    return bestKeysize;
}
