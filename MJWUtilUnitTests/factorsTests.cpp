#include "CppUnitTest.h"
#include "../MJWUtilLib/util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MJWUtilsTests
{
	TEST_CLASS(GCDEuclid) {
public:
		TEST_METHOD(KnownValuesFirstGreater) {
			Assert::AreEqual(12, greatestCommonFactorEuclid(36, 24));
			Assert::AreEqual(5, greatestCommonFactorEuclid(15, 10));
			Assert::AreEqual(0, greatestCommonFactorEuclid(10, 0));
			Assert::AreEqual(6, greatestCommonFactorEuclid(888, 54));
		}

		TEST_METHOD(KnownValuesSecondGreater) {
			Assert::AreEqual(12, greatestCommonFactorEuclid(24, 36));
			Assert::AreEqual(5, greatestCommonFactorEuclid(10, 15));
			Assert::AreEqual(0, greatestCommonFactorEuclid(0, 10));
		}

		TEST_METHOD(ValuesEqual) {
			Assert::AreEqual(24, greatestCommonFactorEuclid(24, 24));
			Assert::AreEqual(11, greatestCommonFactorEuclid(11, 11));
			Assert::AreEqual(0, greatestCommonFactorEuclid(0, 0));
		}
	};

	TEST_CLASS(LCMNaive) {
public:
		TEST_METHOD(KnownValuesFirstGreater) {
			Assert::AreEqual(77, leastCommonMultipleNaive(7, 11));
			Assert::AreEqual(24, leastCommonMultipleNaive(8, 12));
		}

		TEST_METHOD(ValuesEqual) {
			Assert::AreEqual(12, leastCommonMultipleNaive(12, 12));
			Assert::AreEqual(5, leastCommonMultipleNaive(5, 5));
		}
	};
}
