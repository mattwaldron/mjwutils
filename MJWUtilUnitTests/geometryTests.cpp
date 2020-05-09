#include "CppUnitTest.h"
#include "../MJWUtilLib/util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MJWUtilsTests {
    TEST_CLASS(Geometry) {
public:
        TEST_METHOD(CenterOfMass) {
            Assert::IsTrue(Vector2(0, 0) == centerOfMass(std::vector<Vector2>{ { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }}));
        }
    };
}
