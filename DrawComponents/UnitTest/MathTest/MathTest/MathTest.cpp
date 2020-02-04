#include "pch.h"
#include "CppUnitTest.h"

#include "..\\..\\..\\DXGraphicAPI\\Core\\Math\\Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathTest
{
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(ConstructorTest)
		{
			using namespace Math;
			Vector3 v0;
			Vector3 v1(0.0, 0.0, 0.0);
			Assert::IsTrue(v1.IsEqual(v0));

			Vector3 v2(0.2, 0.5, 1.5);
			Assert::IsTrue(v0.IsEqual(Vector3(0.2, 0.5, 1.5)));
		}

		TEST_METHOD(OperatorTest)
		{

		}
	};
}
