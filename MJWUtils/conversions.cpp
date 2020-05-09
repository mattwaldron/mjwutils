#include "util.h"

#include <string>

unsigned char hexCharToByte(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	return 0xFF;
}

char nybbleToChar(unsigned char n) {
	if (n < 10) {
		return '0' + n;
	}
	if (n < 16) {
		return 'a' + n - 10;
	}
	return -1;
}

char byteToBase64Char(unsigned char b) {
	if (b >= 0 && b <= 25) {
		return b + 'A';
	}
	if (b >= 26 && b <= 51) {
		return b - 26 + 'a';
	}
	if (b >= 52 && b <= 61) {
		return b - 52 + '0';
	}
	if (b == 62) {
		return '+';
	}
	if ( b == 63) {
		return '/';
	}
	return -1;
}

unsigned char base64CharToBytes(char c) {
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	}
	if (c >= 'a' && c <= 'z') {
		return 26 + c - 'a';
	}
	if (c >= '0' && c <= '9') {
		return 52 + c - '0';
	}
	if (c == '+') {
		return 62;
	}
	if (c == '/') {
		return 63;
	}
	return 0;
}

std::vector<char> stringToCharVector(const std::string & s) {
	return std::vector<char>(s.begin(), s.end());
}

std::string charVectorToString(const std::vector<char>& bytes) {
	return std::string(bytes.begin(), bytes.end());
}

std::string charVectorToString(const std::vector<unsigned char>& bytes) {
	return std::string(bytes.begin(), bytes.end());
}

std::vector<unsigned char> hexstringToBytes(const std::vector<char> & hex) {
	std::vector<unsigned char> bytes((hex.size() + 1) / 2);
	unsigned int i;
	for (i = 0; i < hex.size() / 2; i++) {
		bytes[i] = (hexCharToByte(hex[i*2]) << 4) + (hexCharToByte(hex[i*2+1]));
	}
	if (hex.size() % 2 == 1) {
		bytes[i] = hexCharToByte(hex[i * 2]) << 4;
	}
	return bytes;
}

std::vector<char> bytesToHexstring(const std::vector<unsigned char> & bytes) {
	std::vector<char> hex(bytes.size() * 2);
	for (int i = 0; i < hex.size(); i++) {
		hex[i] = nybbleToChar(i % 2 == 0
			? bytes[i / 2] >> 4
			: bytes[i / 2] & 0x0F);
	}
	return hex;
}

std::vector<char> bytesToBase64(const std::vector<unsigned char> & bytes) {
	std::vector<char> base64((4 * bytes.size() + 2) / 3);
	unsigned int byteIdx, b64Idx;
	for (byteIdx = 0, b64Idx = 0; b64Idx < (4 * bytes.size() / 3); byteIdx += 3, b64Idx += 4) {
		base64[b64Idx]	    = byteToBase64Char((bytes[byteIdx] & 0xFC) >> 2);
		base64[b64Idx + 1]  = byteToBase64Char(((bytes[byteIdx] & 0x03) << 4)
											+ ((bytes[byteIdx + 1] & 0xF0) >> 4));
		base64[b64Idx + 2]  = byteToBase64Char(((bytes[byteIdx + 1] & 0x0F) << 2)
											+ ((bytes[byteIdx + 2] & 0xC0) >> 6));
		base64[b64Idx + 3]  = byteToBase64Char(bytes[byteIdx + 2] & 0x3F);
	}
	if (bytes.size() % 3 > 0) {
		base64[b64Idx] = byteToBase64Char((bytes[byteIdx] & 0xFC) >> 2);
	}
	if (bytes.size() % 3 > 1) {
		base64[b64Idx + 1] = byteToBase64Char(((bytes[byteIdx] & 0x03) << 4)
												+ ((bytes[byteIdx + 1] & 0xF0) >> 4));
		base64[b64Idx + 2] = byteToBase64Char(((bytes[byteIdx + 1] & 0x0F) << 2)
												+ ((bytes[byteIdx + 2] & 0xC0) >> 6));
	}

	return base64;
}

std::vector<unsigned char> base64ToBytes(const std::vector<char>& base64) {
	std::vector<unsigned char> bytes((3 * base64.size() + 3) / 4);
	unsigned int byteIdx, b64Idx;
	for (byteIdx = 0, b64Idx = 0; byteIdx < (3 * base64.size() / 4); byteIdx += 3, b64Idx += 4) {
		bytes[byteIdx]     = ((base64CharToBytes(base64[b64Idx]) & 0x3F) << 2)
						   + ((base64CharToBytes(base64[b64Idx + 1]) & 0x30) >> 4);
		bytes[byteIdx + 1] = ((base64CharToBytes(base64[b64Idx + 1]) & 0x0F) << 4)
						   + ((base64CharToBytes(base64[b64Idx + 2]) & 0x3C) >> 2);
		bytes[byteIdx + 2] = ((base64CharToBytes(base64[b64Idx + 2]) & 0x03) << 6)
						   + ((base64CharToBytes(base64[b64Idx + 3]) & 0x3F));
	}

	switch (base64.size() % 4) {
	case 0:
		break;
	case 1:
		bytes[byteIdx] = ((base64CharToBytes(base64[b64Idx]) & 0x3F) << 2);
		break;
	case 2:
		bytes[byteIdx] = ((base64CharToBytes(base64[b64Idx]) & 0x3F) << 2)
			+ ((base64CharToBytes(base64[b64Idx + 1]) & 0x3) >> 4);
		bytes[byteIdx + 1] = ((base64CharToBytes(base64[b64Idx + 1]) & 0xF) << 4);
		break;
	case 3:
		bytes[byteIdx] = ((base64CharToBytes(base64[b64Idx]) & 0x3F) << 2)
			+ ((base64CharToBytes(base64[b64Idx + 1]) & 0x3) >> 4);
		bytes[byteIdx + 1] = ((base64CharToBytes(base64[b64Idx + 1]) & 0xF) << 4)
			+ ((base64CharToBytes(base64[b64Idx + 2]) & 0x3C) >> 2);
		bytes[byteIdx + 2] = ((base64CharToBytes(base64[b64Idx + 2]) & 0x03) << 6);
		break;
	}

	return bytes;
}
