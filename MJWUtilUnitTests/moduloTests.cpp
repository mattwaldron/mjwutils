#include "CppUnitTest.h"
#include "../MJWUtilLib/util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MJWUtilsTests {
	TEST_CLASS(PowMod) {
public:
	TEST_METHOD(KnownValues) {
		Assert::AreEqual(4, modularExponent(5, 2, 7));
		Assert::AreEqual(2, modularExponent(2, 5, 10));
		Assert::AreEqual(2, modularExponent(3, 3, 5));
	}
	};
	TEST_CLASS(ModInverse) {

		TEST_METHOD(KnownValues) {
			Assert::AreEqual(442, modularInverse(43, 543));
			Assert::AreEqual(14, modularInverse(11, 17));
			Assert::AreEqual(363, modularInverse(27, 392));
			Assert::AreEqual(9, modularInverse(3, 26));
			Assert::AreEqual(36, modularInverse(15, 77));
		}
	};

	TEST_CLASS(MontgomeryReduction)
	{
		TEST_METHOD(CompareWithNaive) {
			Assert::AreEqual((11 * 7) % 5, montgomeryReduction(11, 7, 5));
			Assert::AreEqual((101 * 97) % 17, montgomeryReduction(101, 97, 17));
			Assert::AreEqual((337 * 377) % 23, montgomeryReduction(337, 377, 23));
		}
	};
}
