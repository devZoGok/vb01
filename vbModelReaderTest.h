#ifndef VB_MODEL_READER_TEST
#define VB_MODEL_READER_TEST

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <string>

namespace vb01{
	class VbModelReader;

	class VbModelReaderTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(VbModelReaderTest);
		CPPUNIT_TEST(testReadSkeletons);
		CPPUNIT_TEST(testReadMeshes);
		CPPUNIT_TEST_SUITE_END();

		public:
			VbModelReaderTest();
			~VbModelReaderTest();
			void setUp();
			void tearDown();
		private:
			void setupSkeleton();
			void testReadSkeletons();
			void setupMesh();
			void testReadMeshes(); 
			void testReadLights(); 

			VbModelReader *modelReader = nullptr;
			std::vector<std::string> skeletonData, meshData;
			int numBones = 0;
	};
}

#endif
