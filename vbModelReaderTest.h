#ifndef VB_MODEL_READER_TEST
#define VB_MODEL_READER_TEST

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <string>

#include "vector.h"
#include "animation.h"

namespace vb01{
	class VbModelReader;

	class VbModelReaderTest;
	typedef VbModelReaderTest vbReader;

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

			struct Bone{
				Bone(std::string name, std::string parent, Vector3 head, Vector3 xAxis, Vector3 yAxis, std::string ikTarget, int ikChainLength){
					this->name = name;
					this->parent = parent;
					this->head = head;
					this->xAxis = xAxis;
					this->yAxis = yAxis;
					this->ikTarget = ikTarget;
					this->ikChainLength = ikChainLength;
				}

				std::string name, parent, ikTarget;
				Vector3 head, xAxis, yAxis;
				int ikChainLength;
			};

			VbModelReader *modelReader = nullptr;
			std::string meshName, skeletonName, meshParent, skeletonParent;
			std::vector<std::string> vertexGroups;
			std::vector<Vector3> vertPos, vertNorm, vertTan, vertBiTan;
			std::vector<Vector2> vertUv;
			std::vector<std::string> skeletonData, meshData;
			std::vector<vbReader::Bone> bones;
			std::vector<std::vector<float>> vertWeights;
			std::vector<Animation> animations;
	};
}

#endif
