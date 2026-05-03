#ifndef ASSET_MANAGER_TEST_H
#define ASSET_MANAGER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class AssetManager;

	class AssetManagerTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(AssetManagerTest);
		CPPUNIT_TEST(testLoad);
		CPPUNIT_TEST_SUITE_END();

		public:
			AssetManagerTest(){}
			void setUp();
			void tearDown();
			void testLoad();
		private:
			AssetManager *assetManager = nullptr;
			std::string recursiveLoadPath, fileLoadPath;
	};
}

#endif
