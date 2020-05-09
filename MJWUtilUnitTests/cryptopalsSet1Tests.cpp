#include "CppUnitTest.h"
#include "../MJWUtilLib/util.h"
#include "openssl/evp.h"
#include "openssl/conf.h"
#include "openssl/err.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MJWUtilsTests {
	TEST_CLASS(CryptoSet1)
	{
	public:
		TEST_METHOD(HexToBase64) {
			auto input = stringToCharVector("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
			std::string output = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
			auto base64 = bytesToBase64(hexstringToBytes(input));
			std::string base64Str = std::string(base64.begin(), base64.end());
			Assert::AreEqual(output, base64Str);
		}

		TEST_METHOD(FixedXOR) {
			auto aa = stringToCharVector("1c0111001f010100061a024b53535009181c");
			auto bb = stringToCharVector("686974207468652062756c6c277320657965");
			std::string expected = "746865206b696420646f6e277420706c6179";

			auto a = hexstringToBytes(aa);
			auto b = hexstringToBytes(bb);

			auto xx = repeatingKeyXor(a, b);
			auto x = bytesToHexstring(xx);
			std::string xstr = std::string(x.begin(), x.end());
			Assert::AreEqual(expected, xstr);
		}

		TEST_METHOD(BestSingleByteXor) {
			auto aa = stringToCharVector("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
			auto a = hexstringToBytes(aa);

			unsigned char cipher;
			determineSingleByteCipher(a, cipher);

			auto message = charVectorToString(singleByteXor(a, cipher));
			Assert::AreEqual((unsigned char)'X', cipher);
			std::cout << message << std::endl; // Debug to check that this is intelligible
		}

		TEST_METHOD(BestLineWithSingleByteXor) {
			std::vector<unsigned char> bestLine;
			double bestScore = 0;
			unsigned char bestCipher = 0;

			std::ifstream in("cryptopals_s1c4_input.txt");
			if (!in) { Assert::Fail(); }

			while (in) {
				std::string str;
				std::getline(in, str);
				auto hexstring = stringToCharVector(str);
				auto bytes = hexstringToBytes(hexstring);
				unsigned char cipher;
				auto score = determineSingleByteCipher(bytes, cipher);
				if (score > bestScore) {
					bestScore = score;
					bestCipher = cipher;
					bestLine = bytes;
				}
			}

			in.close();
			Assert::AreEqual((unsigned char)'5', bestCipher);
			auto message = charVectorToString(singleByteXor(bestLine, bestCipher));
		}

		TEST_METHOD(RepeatingKeyCipher) {
			std::string in("Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal");
			std::vector<unsigned char> key{ 'I', 'C', 'E' };
			std::vector<unsigned char> bytes(in.begin(), in.end());
			auto encrypted = repeatingKeyXor(bytes, key);
			auto output = bytesToHexstring(encrypted);
			Assert::AreEqual(output[0], '0');
			Assert::AreEqual(output[90], '2');
			Assert::AreEqual(output[147], 'f');
		}

		TEST_METHOD(HammingDistance) {
			std::string aa("this is a test");
			std::string bb("wokka wokka!!!");
			auto a = std::vector<unsigned char>(aa.begin(), aa.end());
			auto b = std::vector<unsigned char>(bb.begin(), bb.end());
			Assert::AreEqual(37, hammingDistance(a, b));
		}

		TEST_METHOD(Base64Decode) {
			std::string b64("VGhlIHF1aWNrIGJyb3du");
			std::vector<char> b64vec(b64.begin(), b64.end());
			std::vector<unsigned char> expected({ 0x54, 0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b, 0x20, 0x62, 0x72, 0x6f, 0x77, 0x6e });
			auto decoded = base64ToBytes(b64vec);
			Assert::IsTrue(expected == decoded);
		}

		TEST_METHOD(BreakRepeatingKey) {
			std::ifstream in("cryptopals_s1c6_input.txt");
			std::stringstream ss;
			if (!in) { Assert::Fail(); }

			while (in) {
				std::string str;
				std::getline(in, str);
				ss << str;
			}

			in.close();

			auto allinput = ss.str();

			auto ciphertext = base64ToBytes(std::vector<char>(allinput.begin(), allinput.end()));
			auto keysize = estimateKeysize(ciphertext);

			std::vector<unsigned char> ciphers(keysize);
			for (int k = 0; k < keysize; k++) {
				std::vector<unsigned char> ctslice;
				for (int i = 0; i < ciphertext.size(); i++) {
					if ((i % keysize) == k) {
						ctslice.push_back(ciphertext[i]);
					}
				}
				determineSingleByteCipher(ctslice, ciphers[k]);
			}

			auto plaintext = repeatingKeyXor(ciphertext, ciphers);
			auto ptstr = std::string(plaintext.begin(), plaintext.end());
		}

		TEST_METHOD(AESDecrypt) {
			std::ifstream in("cryptopals_s1c7_input.txt");
			std::stringstream ss;
			if (!in) { Assert::Fail(); }

			while (in) {
				std::string str;
				std::getline(in, str);
				ss << str;
			}

			in.close();

			auto allinput = ss.str();
			auto ciphertext = base64ToBytes(std::vector<char>(allinput.begin(), allinput.end()));

			std::string keyStr = "YELLOW SUBMARINE";
			std::vector<unsigned char> key(keyStr.begin(), keyStr.end());
			std::vector<unsigned char> iv(key.size());
			std::vector<unsigned char> decrypted(ciphertext.size());
			int len;
			int total_len;

			EVP_CIPHER_CTX* ctx;
			if (!(ctx = EVP_CIPHER_CTX_new())) {
				Assert::Fail();
			}

			if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.data(), iv.data())) {
				EVP_CIPHER_CTX_free(ctx);
				Assert::Fail();
			}
			if (1 != EVP_DecryptUpdate(ctx, decrypted.data(), &len, ciphertext.data(), ciphertext.size())) {
				EVP_CIPHER_CTX_free(ctx);
				Assert::Fail();
			}
			total_len = len;
			if (1 != EVP_DecryptFinal_ex(ctx, decrypted.data() + len, &len)) {
				EVP_CIPHER_CTX_free(ctx);
				Assert::Fail();
			}
			total_len += len;
			EVP_CIPHER_CTX_free(ctx);
			decrypted.resize(ciphertext.size());
			auto decryptedStr = std::string(decrypted.begin(), decrypted.end());
			Assert::AreEqual("I'm back", decryptedStr.substr(0, 8).c_str());
			//EXPECT_THAT(decryptedStr, ::testing::HasSubstr("ringin"));
		}

		/*void SHA256(const unsigned char* message, size_t message_len, unsigned char** digest, unsigned int* digest_len)
		{
			*digest_len = 0;
			EVP_MD_CTX* mdctx;

			if ((mdctx = EVP_MD_CTX_new()) == NULL) {
				return;
			}

			if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
				return;
			}

			if (1 != EVP_DigestUpdate(mdctx, message, message_len)) {
				return;
			}

			if ((*digest = (unsigned char*)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL) {
				return;
			}

			if (1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len)) {
				return;
			}

			EVP_MD_CTX_free(mdctx);
		}*/

		TEST_METHOD(FindEBCEncryptedLine) {

			std::ifstream in("cryptopals_s1c8_input.txt");
			const int blocksize = 16;
			if (!in) { Assert::Fail(); }

			std::vector<unsigned char> mostRepeatedLine;
			int mostRepeatedBlocks = 0;

			while (in) {
				std::string str;
				std::getline(in, str);
				auto hexstring = stringToCharVector(str);
				auto bytes = hexstringToBytes(hexstring);
				int repeats = 0;
				std::vector<std::vector<unsigned char>> blocks;
				for (int idx = 0; idx < bytes.size(); idx += blocksize) {
					std::vector<unsigned char> thisBlock(bytes.begin() + idx, bytes.begin() + idx + blocksize);
					if (std::any_of(blocks.begin(), blocks.end(), [thisBlock](std::vector<unsigned char> b) {return thisBlock == b; })) {
						repeats++;
					}
					else {
						blocks.push_back(thisBlock);
					}
				}
				if (repeats > mostRepeatedBlocks) {
					mostRepeatedBlocks = repeats;
					mostRepeatedLine = bytes;
				}
			}

			in.close();

			// inspect bytes with debugger - should be d8806197

		}
	};
}